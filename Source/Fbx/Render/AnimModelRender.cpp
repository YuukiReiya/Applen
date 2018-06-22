#include "../../Device/DeviceManager.h"
#include "AnimModelRender.h"
#include "../Load//ModelLoad.h"
#include <codecvt>

using namespace modelstruct;
using namespace DirectX;
using namespace std;

AnimModelRender::AnimModelRender()
{
	//値の初期化
	animFrame = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* デバイスの初期化とリソースの読み込み
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void AnimModelRender::Init(const std::string filePath)
{
	//テクスチャサンプラーの初期化
	InitSampler();

	//リソース
	InitializeResource(filePath);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* リソースの追加
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT AnimModelRender::InitializeResource(const string filePath)
{
	ModelLoad loader;

	//loderからデータの受け渡し
	loader.Load(filePath);
	vsModel = loader.GetMesh();

	sMInfo.xmLoacalMat = vsModel[0].mat;
	sMInfo.xmLoacalMat.r[3].m128_f32[0] = 0;
	sMInfo.xmLoacalMat.r[3].m128_f32[1] = 0;
	sMInfo.xmLoacalMat.r[3].m128_f32[2] = 0;
	sMInfo.xmLoacalMat.r[3].m128_f32[3] = 1;

	D3D11_SUBRESOURCE_DATA  vbsd;
	SecureZeroMemory(&vbsd, sizeof(D3D11_SUBRESOURCE_DATA));

	//メッシュサイズの保存
	sMInfo.nodeMax = (unsigned)vsModel.size();

	//メッシュの数だけループ
	for (auto &num : vsModel)
	{
		//メッシュ毎の最大頂点数
		num.vertexMax = (unsigned)num.vertex.size();

		std::vector<modelstruct::MODEL_VERTEX> vertex;

		//サブソースの作成
		uint32_t i;
		for (i = 0; i < num.vertexMax; ++i)
		{
			MODEL_VERTEX temp;

			//初期化なのでアニメーション番号とフレーム番号は0で良い
			temp.xfPos.x = num.animation[0][0][i].x;
			temp.xfPos.y = num.animation[0][0][i].y;
			temp.xfPos.z = num.animation[0][0][i].z;

			temp.xfNorm.x = num.vertex[i].xfNorm.x;
			temp.xfNorm.y = num.vertex[i].xfNorm.y;
			temp.xfNorm.z = num.vertex[i].xfNorm.z;

			temp.xfTex.x = num.vertex[i].xfTex.x;
			temp.xfTex.y = num.vertex[i].xfTex.y;

			vertex.push_back(temp);
		}

		vbsd.pSysMem = vertex.data();

		//バーテックスバッファ用デスク作成
		const CD3D11_BUFFER_DESC vbd(sizeof(MODEL_VERTEX) * num.vertexMax, D3D11_BIND_VERTEX_BUFFER);

		//バーテックスバッファ作成
		if (FAILED(DeviceManager::GetInstance().pDevice->CreateBuffer(&vbd, &vbsd, num.pVertexBuffer.GetAddressOf())))
		{
			MessageBox(0, L"バーテックスバッファ作成失敗", 0, MB_OK);
		}

		//サブソースの作成
		uint32_t indeSize = num.vertexMax;
		indexCount = indeSize;
		std::vector<unsigned short> ind;

		for (i = 0; i < indeSize; ++i)
		{
			ind.push_back(static_cast<unsigned short>(i));
		}
		std::reverse(ind.begin(), ind.end());

		D3D11_SUBRESOURCE_DATA ibsd;
		SecureZeroMemory(&ibsd,sizeof(D3D11_SUBRESOURCE_DATA));
		ibsd.pSysMem = ind.data();

		//インデックスバッファ用デスク作成
		CD3D11_BUFFER_DESC ibd(sizeof(unsigned short) * indeSize, D3D11_BIND_INDEX_BUFFER);

		//インデックスバッファ作成
		if (FAILED(DeviceManager::GetInstance().pDevice->CreateBuffer(&ibd, &ibsd, num.pIndexBuffer.GetAddressOf())))
		{
			MessageBox(0, L"インデックスバッファ作成失敗", 0, MB_OK);
		}
	}
	//コンスタントバッファ用デスク作成
	const CD3D11_BUFFER_DESC constantBufferDesc(sizeof(MODEL_SHADER_CBUFF), D3D11_BIND_CONSTANT_BUFFER);
	
	//コンスタントバッファの作成
	if (FAILED(DeviceManager::GetInstance().pDevice->CreateBuffer(&constantBufferDesc, nullptr, pModelCBuff.GetAddressOf())))
	{
		MessageBox(0, L"コンスタントバッファ作成失敗", 0, MB_OK);
	}

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* モデル描画
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void AnimModelRender::Render()
{
	// 行列変換
	MatrixConvert();

	if (sMInfo.slot == SHADER_SLOT::SKELTON)
	{
		sMCBuffer.fAlpha = sMInfo.fAlpha;
	}
	else
	{
		sMCBuffer.fAlpha = 1.0f;
	}

	// Bufferの更新
	DeviceManager::GetInstance().pDeviceContext->UpdateSubresource(
		pModelCBuff.Get(),
		0,
		nullptr,
		&sMCBuffer,
		0,
		0
	);

	//Meshの数ぶん描画
	for (auto &num : vsModel)
	{
		// 各頂点は、VertexPositionColor構造体の1つのインスタンスです。
		const UINT stride = sizeof(MODEL_VERTEX);
		const UINT offset = 0;
		DeviceManager::GetInstance().pDeviceContext->IASetVertexBuffers(
			0,
			1,
			num.pVertexBuffer.GetAddressOf(),
			&stride,
			&offset
		);

		DeviceManager::GetInstance().pDeviceContext->IASetIndexBuffer(
			num.pIndexBuffer.Get(),
			DXGI_FORMAT_R16_UINT, // 各インデックスは1つの16ビット符号なし整数（short)
			0
		);

		//アニメーションの処理
		static D3D11_SUBRESOURCE_DATA vbsd = { 0 };
		vbsd.SysMemPitch = 0;
		vbsd.SysMemSlicePitch = 0;

		vector<MODEL_VERTEX> vertex;

		uint32_t i;
		for (i = 0; i < num.vertexMax; ++i)//ポリゴン分回す
		{
			MODEL_VERTEX temp;
			temp.xfPos.x = num.animation[sMInfo.animNum][animFrame][i].x;
			temp.xfPos.y = num.animation[sMInfo.animNum][animFrame][i].y;
			temp.xfPos.z = num.animation[sMInfo.animNum][animFrame][i].z;

			temp.xfNorm.x = num.vertex[i].xfNorm.x;
			temp.xfNorm.y = num.vertex[i].xfNorm.y;
			temp.xfNorm.z = num.vertex[i].xfNorm.z;

			temp.xfTex.x = num.vertex[i].xfTex.x;
			temp.xfTex.y = num.vertex[i].xfTex.y;

			vertex.push_back(temp);
		}

		vbsd.pSysMem = vertex.data();
		const CD3D11_BUFFER_DESC vbd(sizeof(MODEL_VERTEX) * num.vertexMax, D3D11_BIND_VERTEX_BUFFER);

		//バーテックスバッファの更新
		if (FAILED(DeviceManager::GetInstance().pDevice->CreateBuffer(&vbd, &vbsd, num.pVertexBuffer.GetAddressOf())))
		{
			MessageBox(0, L"バーテックスバッファの更新に失敗", 0, MB_OK);
		}

		if (sMInfo.isAnim) { ++animFrame; }

		//アニメーションフレームの加算
		if (animFrame >= vsModel[sMInfo.animNum].animationMax.size() - 1)//フレームがMAXまでいったらリセット
		{
			animFrame = 0;// FlameMax[animNum] * 1.0f;
		}

		// 頂点シェーダにモデル定数バッファを適用する.
		DeviceManager::GetInstance().pDeviceContext->VSSetConstantBuffers(0,1,pModelCBuff.GetAddressOf());

		// テクスチャを貼る
		BindTexture();

		//オブジェクトを描画

		//抜け色
		uint32_t colorKey = 0xffffffff;
		if (sMInfo.slot == SHADER_SLOT::SKELTON)
		{
			DeviceManager::GetInstance().pDeviceContext->OMSetBlendState(pBlendState.Get(), 0, colorKey);
		}
		DeviceManager::GetInstance().pDeviceContext->DrawIndexedInstanced(indexCount,1,0,0,0);
		DeviceManager::GetInstance().pDeviceContext->OMSetBlendState(0, 0, colorKey);

		// Bindしたテクスチャをリセット(2枚以上使うと次のモデルに影響がでるため)
		ResetTextureBind();
	}

	sMInfo.isAnim = false;
}