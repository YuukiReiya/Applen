#include <fstream>
#include <sstream>
#include <locale> 
#include <codecvt> 
#include <cstdio>
#include "ModelManager.h"
#include "../../Device/DeviceManager.h"
#include "../Render/AnimModelRender.h"
#include "../Render/StaticModelRender.h"

using namespace std;
using namespace DirectX;

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* 初期化
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void ModelManager::InitializeModel()
{
	modelstruct::MODEL_SHADER* defShader = new modelstruct::MODEL_SHADER();
	modelstruct::MODEL_SHADER* skeltonShader = new modelstruct::MODEL_SHADER();
	sMShader.push_back(defShader);
	sMShader.push_back(skeltonShader);
	InitShader();	//シェーダーの作成
	LoadCSVData();	//CSV読み込み
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* CSVから全てのモデルを読み込み
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT ModelManager::LoadCSVData()
{
	// CSVを開いて内容を読み込む
	ifstream ifs(DEFAULT_CSV_PATH);

	if (!ifs)
	{
		MessageBox(0, L"CSVファイルを開けません", 0, MB_OK);
		return E_FAIL;
	}

	string str = "";
	uint32_t csvSize  = 0;
	uint32_t dataSize = 0;

	const uint32_t MAX_NUM  = 11;
	const uint32_t MAX_INFO = 15;
	uint32_t MAX_SIZE = 256;

	string data[MAX_NUM][MAX_INFO];


	// ファイルの中身を一行ずつ読み取る
	while (getline(ifs, str))
	{
		string tmp = "";
		istringstream stream(str);

		// 区切り文字がなくなるまで文字を区切っていく
		while (getline(stream, tmp, ','))
		{
			// 区切られた文字がtmpに入る
			data[csvSize][dataSize] = tmp;
			++dataSize;
		}
		++csvSize;
		dataSize = 0;
	}

	// 1行目が情報なので2行目からデータを格納
	uint32_t i;
	for (i = 1; i < csvSize; ++i, ++dataSize)
	{
		//名前を取得
		string name = data[i][0];

		//アニメーションの有無で扱うクラスを変える
		bool isAnim = atoi(data[i][12].c_str());
		if (isAnim)
		{
			//アニメーション有りのクラス
			pMRender[name] = make_shared<AnimModelRender>();
		}
		else
		{
			//アニメーション無しのクラス
			pMRender[name] = make_shared<StaticModelRender>();
		}

		//Skelton
		int slot = atoi(data[i][13].c_str());
		pMRender[name]->ShaderSlot(slot);
		if (slot == modelstruct::SHADER_SLOT::SKELTON)
		{
			pMRender[name]->SetSkeltonValue((float)atof(data[i][14].c_str()));
		}

		//pathからモデル読み込み
		std::string path = data[i][1];
		pMRender[name]->Init(path);

		//pathからテクスチャ読み込み
		std::wstring tpath = std::wstring(data[i][2].begin(), data[i][2].end());
		pMRender[name]->LoadTexture(tpath.c_str());

		//配列にあるデータをfloatに変換して代入
		XMFLOAT3 temp;
		
		//Pos
		temp.x = (float)atof(data[i][3].c_str());
		temp.y = (float)atof(data[i][4].c_str());
		temp.z = (float)atof(data[i][5].c_str());
		pMRender[name]->SetModelPos(temp);

		//Rot
		temp.x = (float)atof(data[i][6].c_str());
		temp.y = (float)atof(data[i][7].c_str());
		temp.z = (float)atof(data[i][8].c_str());
		pMRender[name]->SetModelRot(temp);

		//Scale
		temp.x = (float)atof(data[i][9].c_str());
		temp.y = (float)atof(data[i][10].c_str());
		temp.z = (float)atof(data[i][11].c_str());
		pMRender[name]->SetModelScale(temp);
	}

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* シェーダー作成
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT ModelManager::InitShader()
{
	//hlslファイル読み込みからシェーダ作成まで
	ID3DBlob *pCompiledShader_ = nullptr;
	ID3DBlob *pErrors_ = nullptr;

	//ブロブからバーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromFile(DEFAULT_SHADER_PATH, NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader_, &pErrors_, NULL)))
	{
		SAFE_RELEASE(pCompiledShader_);
		SAFE_RELEASE(pErrors_);
		MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}

	if (FAILED(DeviceManager::GetInstance().pDevice->CreateVertexShader(pCompiledShader_->GetBufferPointer(), pCompiledShader_->GetBufferSize(), NULL, sMShader[0]->pVertexShader.GetAddressOf())))
	{
		SAFE_RELEASE(pCompiledShader_);
		SAFE_RELEASE(pErrors_);
		MessageBox(0, L"バーテックスシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}

	//ブロブからバーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromFile(SKELTON_SHADER_PATH, NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader_, &pErrors_, NULL)))
	{
		SAFE_RELEASE(pCompiledShader_);
		SAFE_RELEASE(pErrors_);
		MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}

	if (FAILED(DeviceManager::GetInstance().pDevice->CreateVertexShader(pCompiledShader_->GetBufferPointer(), pCompiledShader_->GetBufferSize(), NULL, sMShader[1]->pVertexShader.GetAddressOf())))
	{
		SAFE_RELEASE(pCompiledShader_);
		SAFE_RELEASE(pErrors_);
		MessageBox(0, L"バーテックスシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}

	//頂点インプットレイアウトを定義
	D3D11_INPUT_ELEMENT_DESC vertexDesc_[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT numElements_ = sizeof(vertexDesc_) / sizeof(vertexDesc_[0]);

	//頂点インプットレイアウトを作成
	DeviceManager::GetInstance().pDevice->CreateInputLayout(
		vertexDesc_,
		numElements_,
		pCompiledShader_->GetBufferPointer(),
		pCompiledShader_->GetBufferSize(),
		sMShader[0]->pInputLayout.GetAddressOf()
	);

	DeviceManager::GetInstance().pDevice->CreateInputLayout(
		vertexDesc_,
		numElements_,
		pCompiledShader_->GetBufferPointer(),
		pCompiledShader_->GetBufferSize(),
		sMShader[1]->pInputLayout.GetAddressOf()
	);

	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(DEFAULT_SHADER_PATH, NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader_, &pErrors_, NULL)))
	{
		SAFE_RELEASE(pCompiledShader_);
		SAFE_RELEASE(pErrors_);
		MessageBox(0, L"ピクセルhlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}

	if (FAILED(DeviceManager::GetInstance().pDevice->CreatePixelShader(pCompiledShader_->GetBufferPointer(), pCompiledShader_->GetBufferSize(), NULL,sMShader[0]->pPixcelShader.GetAddressOf())))
	{
		SAFE_RELEASE(pCompiledShader_);
		MessageBox(0, L"ピクセルシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader_);

	if (FAILED(D3DX11CompileFromFile(SKELTON_SHADER_PATH, NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader_, &pErrors_, NULL)))
	{
		SAFE_RELEASE(pCompiledShader_);
		SAFE_RELEASE(pErrors_);
		MessageBox(0, L"ピクセルhlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}

	if (FAILED(DeviceManager::GetInstance().pDevice->CreatePixelShader(pCompiledShader_->GetBufferPointer(), pCompiledShader_->GetBufferSize(), NULL, sMShader[1]->pPixcelShader.GetAddressOf())))
	{
		SAFE_RELEASE(pCompiledShader_);
		MessageBox(0, L"ピクセルシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader_);

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* 描画
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void ModelManager::Render(const std::string name)
{
	int slot = pMRender[name]->GetShaderSlot();

	DeviceManager::GetInstance().pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DeviceManager::GetInstance().pDeviceContext->IASetInputLayout(sMShader[slot]->pInputLayout.Get());

	// 頂点シェーダ
	DeviceManager::GetInstance().pDeviceContext->VSSetShader(
		sMShader[slot]->pVertexShader.Get(),
		nullptr,
		0
	);

	// ピクセルシェーダー
	DeviceManager::GetInstance().pDeviceContext->PSSetShader(
		sMShader[slot]->pPixcelShader.Get(),
		nullptr,
		0
	);

	pMRender[name]->Render();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* 解放
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void ModelManager::Release()
{
	SAFE_DELETE_CONTAINER(pMRender);
}