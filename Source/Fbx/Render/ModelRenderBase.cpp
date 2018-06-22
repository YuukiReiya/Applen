#include "ModelRenderBase.h"
#include "../../Device/DeviceManager.h"
#include "../../Camera/CameraManager.h"

using namespace DirectX;

ModelRenderBase::ModelRenderBase()
{
	texCounter = 1;
	animSpeed = 1;

	sMInfo.animNum = 0;
	sMInfo.xfModelPos = { 0.0f,0.0f,0.0f };
	sMInfo.xfModelRot = { 1.0f,1.0f,0.0f };
	sMInfo.xfModelScale = { 1.0f,1.0f,1.0f };
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* �e�N�X�`���̃T���v���[���쐬
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT ModelRenderBase::InitSampler()
{
	//�e�N�X�`���[�p�T���v���[�쐬
	D3D11_SAMPLER_DESC sd;
	SecureZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	//sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;

	if (FAILED(DeviceManager::GetInstance().pDevice->CreateSamplerState(&sd, pSamplerState.GetAddressOf())))
	{
		MessageBox(0, L"�T���v���[�̏������Ɏ��s���܂���", 0, MB_OK);
		return E_FAIL;
	};

	//���߂�����ꍇ
	if (sMInfo.slot == modelstruct::SHADER_SLOT::SKELTON)
	{
		D3D11_BLEND_DESC bd;
		SecureZeroMemory(&bd, sizeof(bd));

		bd.RenderTarget[0].BlendEnable = true;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		
		if (FAILED(DeviceManager::GetInstance().pDevice->CreateBlendState(&bd, pBlendState.GetAddressOf())))
		{
			MessageBox(0, L"BlendState�쐬�Ɏ��s���܂���", 0, MB_OK | MB_ICONWARNING);
			return E_FAIL;
		}
	}

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* Texture�f�[�^���쐬
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT ModelRenderBase::LoadTexture(const wchar_t* texPath)
{
	//�e�N�X�`���ǂݍ���
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(DeviceManager::GetInstance().pDevice.Get(), texPath, 0, 0, &pTexture, 0)))
	{
		MessageBox(0, L"�e�N�X�`���̍쐬�Ɏ��s���܂���", 0, MB_OK);
		return E_FAIL;
	};

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* Texture�f�[�^��؂�ւ�
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void ModelRenderBase::SetTexture(ID3D11ShaderResourceView* usetex)
{
	pTexture = usetex;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* Texture�f�[�^���o�C���h(���Z�b�g�͕�)
* �p�[�c���Ɏg���e�N�X�`�����ς��ꍇ�͂��̊֐��������ƃG���[
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void ModelRenderBase::BindTexture()
{
	DeviceManager::GetInstance().pDeviceContext->PSSetSamplers(0, 1, pSamplerState.GetAddressOf());
	DeviceManager::GetInstance().pDeviceContext->PSSetShaderResources(0, 1, &pTexture);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* Resource�����Z�b�g
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void ModelRenderBase::ResetTextureBind()
{
	int i;
	for (i = 0; 1 < texCounter; ++i)
	{
		//�����l��1�Ȃ���1���Z
		DeviceManager::GetInstance().pDeviceContext->PSSetShaderResources(0, i + 1, nullptr);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* �}�g���b�N�X�ϊ�
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void ModelRenderBase::MatrixConvert()
{
	XMMATRIX tranMat  = DirectX::XMMatrixTranslation(sMInfo.xfModelPos.x, sMInfo.xfModelPos.y, sMInfo.xfModelPos.z);
	XMMATRIX rotMat	  = DirectX::XMMatrixRotationRollPitchYaw(sMInfo.xfModelRot.x, sMInfo.xfModelRot.y, sMInfo.xfModelRot.z);
	XMMATRIX scaleMat = DirectX::XMMatrixScaling(sMInfo.xfModelScale.x, sMInfo.xfModelScale.y, sMInfo.xfModelScale.z);

	XMMATRIX mat = sMInfo.xmLoacalMat * rotMat;
	modelTransform = scaleMat * rotMat * tranMat * CameraManager::GetInstance().xmView * CameraManager::GetInstance().xmProj;
	XMMATRIX mtw = modelTransform;

}