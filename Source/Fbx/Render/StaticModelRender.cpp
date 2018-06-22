#include "StaticModelRender.h"
#include "../Load//ModelLoad.h"
#include "../../Device/DeviceManager.h"

using namespace modelstruct;
using namespace DirectX;
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* デバイスの初期化とリソースの読み込み
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void StaticModelRender::Init(const std::string filePath)
{
	//リソース
	InitializeResource(filePath);

	//テクスチャサンプラーの初期化
	InitSampler();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* リソースの追加
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT StaticModelRender::InitializeResource(const std::string filePath)
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

	//メッシュサイズの保存
	sMInfo.nodeMax = (unsigned)vsModel.size();

	D3D11_SUBRESOURCE_DATA  vbsd;
	SecureZeroMemory(&vbsd, sizeof(D3D11_SUBRESOURCE_DATA));

	//メッシュの数だけループ
	for (auto &num : vsModel)
	{
		//メッシュ毎の最大頂点数
		num.vertexMax = (unsigned)num.vertex.size();

		vector<modelstruct::MODEL_VERTEX> vertex;

		//サブソースの作成
		uint32_t i;
		for (i = 0; i < num.vertexMax; ++i)
		{
			MODEL_VERTEX temp;

			temp.xfPos.x = num.vertex[i].xfPos.x;
			temp.xfPos.y = num.vertex[i].xfPos.y;
			temp.xfPos.z = num.vertex[i].xfPos.z;

			temp.xfNorm.x = num.vertex[i].xfNorm.x;
			temp.xfNorm.y = num.vertex[i].xfNorm.y;
			temp.xfNorm.z = num.vertex[i].xfNorm.z;

			temp.xfTex.x = num.vertex[i].xfTex.x;
			temp.xfTex.y = num.vertex[i].xfTex.y;

			vertex.push_back(temp);
		}

		vbsd.pSysMem = vertex.data();

		//バーテックスバッファ用デスクの作成
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
		SecureZeroMemory(&ibsd, sizeof(D3D11_SUBRESOURCE_DATA));
		ibsd.pSysMem = ind.data();

		//インデックスバッファ用デスクの作成
		CD3D11_BUFFER_DESC ibd(sizeof(unsigned short) * indeSize, D3D11_BIND_INDEX_BUFFER);

		//インデックスバッファの作成
		if (FAILED(DeviceManager::GetInstance().pDevice->CreateBuffer(&ibd, &ibsd, num.pIndexBuffer.GetAddressOf())))
		{
			MessageBox(0, L"インデックスバッファの作成に失敗", 0, MB_OK);
		}
	}
	
	//コンスタントバッファ用デスクの作成
	D3D11_BUFFER_DESC cb;
	SecureZeroMemory(&cb, sizeof(cb));
	cb.ByteWidth = sizeof(MODEL_SHADER_CBUFF);
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//コンスタントバッファの作成
	if(FAILED(DeviceManager::GetInstance().pDevice->CreateBuffer(&cb,nullptr,&pModelCBuff)))
	{
		MessageBox(0, L"コンスタントバッファ作成失敗", 0, MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* モデル描画
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void StaticModelRender::Render()
{
	// 行列変換
	MatrixConvert();

	//Meshの数ぶん描画
	for (auto &num : vsModel)
	{
		// バーテックスバッファをセット
		const UINT stride = sizeof(MODEL_VERTEX);
		const UINT offset = 0;
		DeviceManager::GetInstance().pDeviceContext->IASetVertexBuffers(
			0,
			1,
			num.pVertexBuffer.GetAddressOf(),
			&stride,
			&offset
		);

		//インデックスバッファをセット
		DeviceManager::GetInstance().pDeviceContext->IASetIndexBuffer(
			num.pIndexBuffer.Get(),
			DXGI_FORMAT_R16_UINT, // 各インデックスは1つの16ビット符号なし整数（short)
			0
		);

		D3D11_MAPPED_SUBRESOURCE pData;
		SecureZeroMemory(&pData, sizeof(pData));

		MODEL_SHADER_CBUFF sg;
		SecureZeroMemory(&sg, sizeof(sg));

		//map/unmap
		if (SUCCEEDED(DeviceManager::GetInstance().pDeviceContext->Map(pModelCBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			//ワールド行列
			XMMATRIX m = modelTransform;
			sg.xmWorld = XMMatrixTranspose(m);

			if (sMInfo.slot == SHADER_SLOT::SKELTON)
			{
				sg.fAlpha = sMInfo.fAlpha;
			}
			else
			{
				sg.fAlpha = 1.0f;
			}

			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(MODEL_SHADER_CBUFF));
			DeviceManager::GetInstance().pDeviceContext->Unmap(pModelCBuff, 0);
		}
		else
		{
			MessageBox(0,L"サブリソースデータを読み込めませんでした",0, MB_OK);
		}

		// 頂点シェーダにモデル定数バッファを適用する.
		DeviceManager::GetInstance().pDeviceContext->VSSetConstantBuffers(0,1,&pModelCBuff);

		// テクスチャを貼る
		BindTexture();

		//オブジェクトを描画
		uint32_t colorKey = 0xffffffff;
		if(sMInfo.slot == SHADER_SLOT::SKELTON)
		{
			DeviceManager::GetInstance().pDeviceContext->OMSetBlendState(pBlendState.Get(), 0, colorKey);
		}
		DeviceManager::GetInstance().pDeviceContext->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);
		DeviceManager::GetInstance().pDeviceContext->OMSetBlendState(0, 0, colorKey);

		// Bindしたテクスチャをリセット(2枚以上使うと次のモデルに影響がでるため)
		ResetTextureBind();
	}
}