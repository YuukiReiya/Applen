#include <fstream>
#include <sstream>
#include <locale> 
#include <codecvt> 
#include <cstdio>
#include "TexesTake.h"
#include "../../Device/DeviceManager.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* CSVから画像パスを読み込む
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT TexesTake::TexesCSVLoad(const char* path)
{
	// CSVを開いて内容を読み込む
	ifstream ifs(path);

	if (!ifs)
	{
		MessageBox(0, L"CSVファイルを開けません", 0, MB_OK);
		return E_FAIL;
	}

	string str = "";
	uint32_t csvSize = 0;
	uint32_t dataSize = 0;

	const uint32_t MAX_NUM = 21;
	const uint32_t MAX_INFO = 1;
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
		//パスを取得
		std::string path = std::string(data[i][0].begin(), data[i][0].end());

		//テクスチャを読み込み
		LoadUVTex(path.c_str());
	}

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* 画像をvectorに格納
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT TexesTake::LoadUVTex(const char* path)
{
	ID3D11ShaderResourceView* temp;

	//テクスチャ読み込み
	if (FAILED(D3DX11CreateShaderResourceViewFromFileA(DeviceManager::GetInstance().pDevice.Get(), path, 0, 0, &temp, 0)))
	{
		MessageBox(0, L"テクスチャの作成に失敗しました", 0, MB_OK);
		return E_FAIL;
	};

	vTex.push_back(temp);

	return S_OK;
}