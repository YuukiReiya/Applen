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
* CSV����摜�p�X��ǂݍ���
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT TexesTake::TexesCSVLoad(const char* path)
{
	// CSV���J���ē��e��ǂݍ���
	ifstream ifs(path);

	if (!ifs)
	{
		MessageBox(0, L"CSV�t�@�C�����J���܂���", 0, MB_OK);
		return E_FAIL;
	}

	string str = "";
	uint32_t csvSize = 0;
	uint32_t dataSize = 0;

	const uint32_t MAX_NUM = 21;
	const uint32_t MAX_INFO = 1;
	uint32_t MAX_SIZE = 256;

	string data[MAX_NUM][MAX_INFO];

	// �t�@�C���̒��g����s���ǂݎ��
	while (getline(ifs, str))
	{
		string tmp = "";
		istringstream stream(str);

		// ��؂蕶�����Ȃ��Ȃ�܂ŕ�������؂��Ă���
		while (getline(stream, tmp, ','))
		{
			// ��؂�ꂽ������tmp�ɓ���
			data[csvSize][dataSize] = tmp;
			++dataSize;
		}
		++csvSize;
		dataSize = 0;
	}

	// 1�s�ڂ����Ȃ̂�2�s�ڂ���f�[�^���i�[
	uint32_t i;
	for (i = 1; i < csvSize; ++i, ++dataSize)
	{
		//�p�X���擾
		std::string path = std::string(data[i][0].begin(), data[i][0].end());

		//�e�N�X�`����ǂݍ���
		LoadUVTex(path.c_str());
	}

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* �摜��vector�Ɋi�[
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT TexesTake::LoadUVTex(const char* path)
{
	ID3D11ShaderResourceView* temp;

	//�e�N�X�`���ǂݍ���
	if (FAILED(D3DX11CreateShaderResourceViewFromFileA(DeviceManager::GetInstance().pDevice.Get(), path, 0, 0, &temp, 0)))
	{
		MessageBox(0, L"�e�N�X�`���̍쐬�Ɏ��s���܂���", 0, MB_OK);
		return E_FAIL;
	};

	vTex.push_back(temp);

	return S_OK;
}