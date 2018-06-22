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
* ������
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void ModelManager::InitializeModel()
{
	modelstruct::MODEL_SHADER* defShader = new modelstruct::MODEL_SHADER();
	modelstruct::MODEL_SHADER* skeltonShader = new modelstruct::MODEL_SHADER();
	sMShader.push_back(defShader);
	sMShader.push_back(skeltonShader);
	InitShader();	//�V�F�[�_�[�̍쐬
	LoadCSVData();	//CSV�ǂݍ���
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* CSV����S�Ẵ��f����ǂݍ���
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT ModelManager::LoadCSVData()
{
	// CSV���J���ē��e��ǂݍ���
	ifstream ifs(DEFAULT_CSV_PATH);

	if (!ifs)
	{
		MessageBox(0, L"CSV�t�@�C�����J���܂���", 0, MB_OK);
		return E_FAIL;
	}

	string str = "";
	uint32_t csvSize  = 0;
	uint32_t dataSize = 0;

	const uint32_t MAX_NUM  = 11;
	const uint32_t MAX_INFO = 15;
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
		//���O���擾
		string name = data[i][0];

		//�A�j���[�V�����̗L���ň����N���X��ς���
		bool isAnim = atoi(data[i][12].c_str());
		if (isAnim)
		{
			//�A�j���[�V�����L��̃N���X
			pMRender[name] = make_shared<AnimModelRender>();
		}
		else
		{
			//�A�j���[�V���������̃N���X
			pMRender[name] = make_shared<StaticModelRender>();
		}

		//Skelton
		int slot = atoi(data[i][13].c_str());
		pMRender[name]->ShaderSlot(slot);
		if (slot == modelstruct::SHADER_SLOT::SKELTON)
		{
			pMRender[name]->SetSkeltonValue((float)atof(data[i][14].c_str()));
		}

		//path���烂�f���ǂݍ���
		std::string path = data[i][1];
		pMRender[name]->Init(path);

		//path����e�N�X�`���ǂݍ���
		std::wstring tpath = std::wstring(data[i][2].begin(), data[i][2].end());
		pMRender[name]->LoadTexture(tpath.c_str());

		//�z��ɂ���f�[�^��float�ɕϊ����đ��
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
* �V�F�[�_�[�쐬
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT ModelManager::InitShader()
{
	//hlsl�t�@�C���ǂݍ��݂���V�F�[�_�쐬�܂�
	ID3DBlob *pCompiledShader_ = nullptr;
	ID3DBlob *pErrors_ = nullptr;

	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(DEFAULT_SHADER_PATH, NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader_, &pErrors_, NULL)))
	{
		SAFE_RELEASE(pCompiledShader_);
		SAFE_RELEASE(pErrors_);
		MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}

	if (FAILED(DeviceManager::GetInstance().pDevice->CreateVertexShader(pCompiledShader_->GetBufferPointer(), pCompiledShader_->GetBufferSize(), NULL, sMShader[0]->pVertexShader.GetAddressOf())))
	{
		SAFE_RELEASE(pCompiledShader_);
		SAFE_RELEASE(pErrors_);
		MessageBox(0, L"�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}

	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(SKELTON_SHADER_PATH, NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader_, &pErrors_, NULL)))
	{
		SAFE_RELEASE(pCompiledShader_);
		SAFE_RELEASE(pErrors_);
		MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}

	if (FAILED(DeviceManager::GetInstance().pDevice->CreateVertexShader(pCompiledShader_->GetBufferPointer(), pCompiledShader_->GetBufferSize(), NULL, sMShader[1]->pVertexShader.GetAddressOf())))
	{
		SAFE_RELEASE(pCompiledShader_);
		SAFE_RELEASE(pErrors_);
		MessageBox(0, L"�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}

	//���_�C���v�b�g���C�A�E�g���`
	D3D11_INPUT_ELEMENT_DESC vertexDesc_[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT numElements_ = sizeof(vertexDesc_) / sizeof(vertexDesc_[0]);

	//���_�C���v�b�g���C�A�E�g���쐬
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

	//�u���u����s�N�Z���V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(DEFAULT_SHADER_PATH, NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader_, &pErrors_, NULL)))
	{
		SAFE_RELEASE(pCompiledShader_);
		SAFE_RELEASE(pErrors_);
		MessageBox(0, L"�s�N�Z��hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}

	if (FAILED(DeviceManager::GetInstance().pDevice->CreatePixelShader(pCompiledShader_->GetBufferPointer(), pCompiledShader_->GetBufferSize(), NULL,sMShader[0]->pPixcelShader.GetAddressOf())))
	{
		SAFE_RELEASE(pCompiledShader_);
		MessageBox(0, L"�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader_);

	if (FAILED(D3DX11CompileFromFile(SKELTON_SHADER_PATH, NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader_, &pErrors_, NULL)))
	{
		SAFE_RELEASE(pCompiledShader_);
		SAFE_RELEASE(pErrors_);
		MessageBox(0, L"�s�N�Z��hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}

	if (FAILED(DeviceManager::GetInstance().pDevice->CreatePixelShader(pCompiledShader_->GetBufferPointer(), pCompiledShader_->GetBufferSize(), NULL, sMShader[1]->pPixcelShader.GetAddressOf())))
	{
		SAFE_RELEASE(pCompiledShader_);
		MessageBox(0, L"�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader_);

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* �`��
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void ModelManager::Render(const std::string name)
{
	int slot = pMRender[name]->GetShaderSlot();

	DeviceManager::GetInstance().pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DeviceManager::GetInstance().pDeviceContext->IASetInputLayout(sMShader[slot]->pInputLayout.Get());

	// ���_�V�F�[�_
	DeviceManager::GetInstance().pDeviceContext->VSSetShader(
		sMShader[slot]->pVertexShader.Get(),
		nullptr,
		0
	);

	// �s�N�Z���V�F�[�_�[
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
* ���
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void ModelManager::Release()
{
	SAFE_DELETE_CONTAINER(pMRender);
}