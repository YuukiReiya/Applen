#include "SpriteBase.h"
#include "../Camera/CameraManager.h"

using namespace DirectX;

SpriteBase::SpriteBase(){}

SpriteBase::~SpriteBase()
{
	SAFE_RELEASE(pConstantBuffer);
	SAFE_DELETE(texData);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* Resource�ƍ\���̂̏�����
* @param (td) �l�n���p
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT SpriteBase::Init(TEXTURE_DATA* td)
{
	texData = new TEXTURE_DATA;
	texData = td;

	//�摜�\���ʒu�w��@�f�t�H���g�͑S��
	float l, r, t, b;
	l = t = 0.0f;
	r = b = 1.0f;

	SimpleVertex vertices[] =
	{
		XMFLOAT3(0.0f,texData->xfSize.y, 0.0f),			XMFLOAT2(l,t),//���_1,
		XMFLOAT3(0.0f, 0.0f, 0.0f),						XMFLOAT2(l,b),//���_2
		XMFLOAT3(texData->xfSize.x,texData->xfSize.y, 0.0f),XMFLOAT2(r,t),//���_3
		XMFLOAT3(texData->xfSize.x,0.0f, 0.0f),			XMFLOAT2(r,b),//���_4
	};

	D3D11_BUFFER_DESC bd;
	SecureZeroMemory(&bd,sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(vertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	//�T�u���\�[�X�̏������f�[�^���`
	D3D11_SUBRESOURCE_DATA sb;
	SecureZeroMemory(&sb, sizeof(sb));
	sb.pSysMem = vertices;
	if (FAILED(DeviceManager::GetInstance().pDevice->CreateBuffer(&bd, &sb, pVertexBuffer.GetAddressOf())))
	{
		MessageBox(0, L"�T�u���\�[�X�̍쐬�Ɏ��s���܂���", 0, MB_OK | MB_ICONWARNING);
		return E_FAIL;
	}

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	uint32_t stride = sizeof(SimpleVertex);
	uint32_t offset = 0;
	DeviceManager::GetInstance().pDeviceContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);

	//�e�N�X�`���[�p�T���v���[�쐬
	D3D11_SAMPLER_DESC sd;
	SecureZeroMemory(&sd, sizeof(sd));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	if(FAILED(DeviceManager::GetInstance().pDevice->CreateSamplerState(&sd, pSamplerState.GetAddressOf())))
	{
		MessageBox(0, L"SampleState�쐬�Ɏ��s���܂���", 0, MB_OK | MB_ICONWARNING);
		return E_FAIL;
	}

	//�e�N�X�`���[���쐬
	if(FAILED(D3DX11CreateShaderResourceViewFromFileA(DeviceManager::GetInstance().pDevice.Get(), texData->filePath, 0, 0, pTexture.GetAddressOf(), 0)))
	{
		MessageBox(0, L"�e�N�X�`���ǂݍ��݂Ɏ��s���܂���", 0, MB_OK | MB_ICONWARNING);
		return E_FAIL;
	}

	//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	ID3DBlob *pCompiledShader = nullptr;
	ID3DBlob *pErrors = nullptr;

	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if(FAILED(D3DX11CompileFromFile(L"Resource/Hlsl/SpriteTex.hlsl", 0, 0, "VS", "vs_5_0", 0, 0, 0, &pCompiledShader, &pErrors, 0)))
	{
		MessageBox(0, L"VS�ǂݍ��݂Ɏ��s���܂���", 0, MB_OK | MB_ICONWARNING);
		return E_FAIL;
	}

	if(FAILED(DeviceManager::GetInstance().pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), 0, pVertexShader.GetAddressOf())))
	{
		MessageBox(0, L"VS�쐬�Ɏ��s���܂���", 0, MB_OK | MB_ICONWARNING);
		return E_FAIL;
	}

	//���_�C���v�b�g���C�A�E�g���`
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	uint32_t numElements = sizeof(layout) / sizeof(layout[0]);

	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(DeviceManager::GetInstance().pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), pInputLayout.GetAddressOf())))
	{
		MessageBox(0, L"���_�C���v�b�g���C�A�E�g�̍쐬�Ɏ��s���܂���", 0, MB_OK | MB_ICONWARNING);
		return E_FAIL;
	}

	//�u���u����s�N�Z���V�F�[�_�[�쐬
	if(FAILED(D3DX11CompileFromFile(L"Resource/Hlsl/SpriteTex.hlsl", 0, 0, "PS", "ps_5_0", 0, 0, 0, &pCompiledShader, &pErrors, 0)))
	{
		MessageBox(0, L"PS�ǂݍ��݂Ɏ��s���܂���", 0, MB_OK | MB_ICONWARNING);
		return E_FAIL;
	}

	if(FAILED(DeviceManager::GetInstance().pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), 0, pPixelShader.GetAddressOf())))
	{
		MessageBox(0, L"PS�쐬�Ɏ��s���܂���", 0, MB_OK | MB_ICONWARNING);
		return E_FAIL;
	}

	SAFE_RELEASE(pCompiledShader);

	//�R���X�^���g�o�b�t�@�[�쐬�@�����ł͕ϊ��s��n���p
	D3D11_BUFFER_DESC cb;
	SecureZeroMemory(&cb, sizeof(cb));
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SPRITE_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if(FAILED(DeviceManager::GetInstance().pDevice->CreateBuffer(&cb, 0, &pConstantBuffer)))
	{
		MessageBox(0, L"�R���X�^���g�o�b�t�@�쐬�Ɏ��s���܂���", 0, MB_OK | MB_ICONWARNING);
		return E_FAIL;
	}

	D3D11_BLEND_DESC bd2;
	SecureZeroMemory(&bd2, sizeof(bd2));
	bd2.RenderTarget[0].BlendEnable = true;
	bd2.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd2.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd2.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd2.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd2.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd2.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd2.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if(FAILED(DeviceManager::GetInstance().pDevice->CreateBlendState(&bd2, pBlendState.GetAddressOf())))
	{
		MessageBox(0, L"BlendState�쐬�Ɏ��s���܂���", 0, MB_OK | MB_ICONWARNING);
		return E_FAIL;
	}

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* �ʒu�A��]�A�X�P�[���̂ݍX�V�\
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void SpriteBase::Update(TEXTURE_DATA* td)
{
	texData->xfPos = td->xfPos;
	texData->xfRot = td->xfRot;
	texData->xfScale = td->xfScale;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* �`��
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void SpriteBase::Render()
{
	//�g�|���W�[
	DeviceManager::GetInstance().pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//���_�C���v�b�g���C�A�E�g���Z�b�g
	DeviceManager::GetInstance().pDeviceContext->IASetInputLayout(pInputLayout.Get());

	//�g�p����V�F�[�_�[�̓o�^
	DeviceManager::GetInstance().pDeviceContext->VSSetShader(pVertexShader.Get(), 0, 0);
	DeviceManager::GetInstance().pDeviceContext->PSSetShader(pPixelShader.Get(), 0, 0);

	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	DeviceManager::GetInstance().pDeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	DeviceManager::GetInstance().pDeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	DeviceManager::GetInstance().pDeviceContext->PSSetSamplers(0, 1, pSamplerState.GetAddressOf());
	DeviceManager::GetInstance().pDeviceContext->PSSetShaderResources(0, 1, pTexture.GetAddressOf());

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	SecureZeroMemory(&pData, sizeof(pData));

	SPRITE_CONSTANT_BUFFER		cb;
	SecureZeroMemory(&cb, sizeof(cb));

	XMMATRIX xmWorld = MatrixConvert();

	if (SUCCEEDED(DeviceManager::GetInstance().pDeviceContext->Map(pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{	
		//���[���h�A�J�����A�ˉe�s���n��
		XMMATRIX m = xmWorld*CameraManager::GetInstance().xmView*CameraManager::GetInstance().xmProj;
		m = XMMatrixTranspose(m);
		cb.xmWVP = m;

		//�J���[��n��
		cb.xfColor = texData->xfColor;
		//�����x��n��
		cb.xfAlpha.x = texData->xfColor.w;

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		DeviceManager::GetInstance().pDeviceContext->Unmap(pConstantBuffer, 0);
	}

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	uint32_t stride = sizeof(SimpleVertex);
	uint32_t offset = 0;
	DeviceManager::GetInstance().pDeviceContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);

	//�����F
	uint32_t colorKey = 0xffffffff;
	DeviceManager::GetInstance().pDeviceContext->OMSetBlendState(pBlendState.Get(), 0, colorKey);
	DeviceManager::GetInstance().pDeviceContext->Draw(4, 0);
	DeviceManager::GetInstance().pDeviceContext->OMSetBlendState(0, 0, colorKey);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* ���W�ϊ�
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
XMMATRIX SpriteBase::MatrixConvert()
{
	//���[���h�ϊ�
	XMMATRIX xmWorld, xmTran, xmRot, xmScale;
	xmWorld = XMMatrixIdentity();
	xmTran = CameraFollow();

	xmScale = XMMatrixScaling(texData->xfScale.x / TEX_SCALE_X, texData->xfScale.y / TEX_SCALE_Y, TEX_SCALE_Z);
	xmRot = XMMatrixRotationRollPitchYaw(texData->xfRot.x, texData->xfRot.y, texData->xfRot.z);
	xmWorld = xmScale * xmRot * xmTran;

	return xmWorld;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* �J�����Ǐ]
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
XMMATRIX SpriteBase::CameraFollow()
{
	XMMATRIX xmTran;

	if (texData->isFollow)
	{
		auto temp = CameraManager::GetInstance().GetEye();
		xmTran = XMMatrixTranslation(texData->xfPos.x +temp.x, texData->xfPos.y + temp.y, texData->xfPos.z +temp.z);
	}
	else
	{
		xmTran = XMMatrixTranslation(texData->xfPos.x, texData->xfPos.y, texData->xfPos.z);
	}

	return xmTran;
}