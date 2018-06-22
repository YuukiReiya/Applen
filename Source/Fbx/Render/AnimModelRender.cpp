#include "../../Device/DeviceManager.h"
#include "AnimModelRender.h"
#include "../Load//ModelLoad.h"
#include <codecvt>

using namespace modelstruct;
using namespace DirectX;
using namespace std;

AnimModelRender::AnimModelRender()
{
	//�l�̏�����
	animFrame = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* �f�o�C�X�̏������ƃ��\�[�X�̓ǂݍ���
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void AnimModelRender::Init(const std::string filePath)
{
	//�e�N�X�`���T���v���[�̏�����
	InitSampler();

	//���\�[�X
	InitializeResource(filePath);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* ���\�[�X�̒ǉ�
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT AnimModelRender::InitializeResource(const string filePath)
{
	ModelLoad loader;

	//loder����f�[�^�̎󂯓n��
	loader.Load(filePath);
	vsModel = loader.GetMesh();

	sMInfo.xmLoacalMat = vsModel[0].mat;
	sMInfo.xmLoacalMat.r[3].m128_f32[0] = 0;
	sMInfo.xmLoacalMat.r[3].m128_f32[1] = 0;
	sMInfo.xmLoacalMat.r[3].m128_f32[2] = 0;
	sMInfo.xmLoacalMat.r[3].m128_f32[3] = 1;

	D3D11_SUBRESOURCE_DATA  vbsd;
	SecureZeroMemory(&vbsd, sizeof(D3D11_SUBRESOURCE_DATA));

	//���b�V���T�C�Y�̕ۑ�
	sMInfo.nodeMax = (unsigned)vsModel.size();

	//���b�V���̐��������[�v
	for (auto &num : vsModel)
	{
		//���b�V�����̍ő咸�_��
		num.vertexMax = (unsigned)num.vertex.size();

		std::vector<modelstruct::MODEL_VERTEX> vertex;

		//�T�u�\�[�X�̍쐬
		uint32_t i;
		for (i = 0; i < num.vertexMax; ++i)
		{
			MODEL_VERTEX temp;

			//�������Ȃ̂ŃA�j���[�V�����ԍ��ƃt���[���ԍ���0�ŗǂ�
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

		//�o�[�e�b�N�X�o�b�t�@�p�f�X�N�쐬
		const CD3D11_BUFFER_DESC vbd(sizeof(MODEL_VERTEX) * num.vertexMax, D3D11_BIND_VERTEX_BUFFER);

		//�o�[�e�b�N�X�o�b�t�@�쐬
		if (FAILED(DeviceManager::GetInstance().pDevice->CreateBuffer(&vbd, &vbsd, num.pVertexBuffer.GetAddressOf())))
		{
			MessageBox(0, L"�o�[�e�b�N�X�o�b�t�@�쐬���s", 0, MB_OK);
		}

		//�T�u�\�[�X�̍쐬
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

		//�C���f�b�N�X�o�b�t�@�p�f�X�N�쐬
		CD3D11_BUFFER_DESC ibd(sizeof(unsigned short) * indeSize, D3D11_BIND_INDEX_BUFFER);

		//�C���f�b�N�X�o�b�t�@�쐬
		if (FAILED(DeviceManager::GetInstance().pDevice->CreateBuffer(&ibd, &ibsd, num.pIndexBuffer.GetAddressOf())))
		{
			MessageBox(0, L"�C���f�b�N�X�o�b�t�@�쐬���s", 0, MB_OK);
		}
	}
	//�R���X�^���g�o�b�t�@�p�f�X�N�쐬
	const CD3D11_BUFFER_DESC constantBufferDesc(sizeof(MODEL_SHADER_CBUFF), D3D11_BIND_CONSTANT_BUFFER);
	
	//�R���X�^���g�o�b�t�@�̍쐬
	if (FAILED(DeviceManager::GetInstance().pDevice->CreateBuffer(&constantBufferDesc, nullptr, pModelCBuff.GetAddressOf())))
	{
		MessageBox(0, L"�R���X�^���g�o�b�t�@�쐬���s", 0, MB_OK);
	}

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* ���f���`��
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void AnimModelRender::Render()
{
	// �s��ϊ�
	MatrixConvert();

	if (sMInfo.slot == SHADER_SLOT::SKELTON)
	{
		sMCBuffer.fAlpha = sMInfo.fAlpha;
	}
	else
	{
		sMCBuffer.fAlpha = 1.0f;
	}

	// Buffer�̍X�V
	DeviceManager::GetInstance().pDeviceContext->UpdateSubresource(
		pModelCBuff.Get(),
		0,
		nullptr,
		&sMCBuffer,
		0,
		0
	);

	//Mesh�̐��Ԃ�`��
	for (auto &num : vsModel)
	{
		// �e���_�́AVertexPositionColor�\���̂�1�̃C���X�^���X�ł��B
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
			DXGI_FORMAT_R16_UINT, // �e�C���f�b�N�X��1��16�r�b�g�����Ȃ������ishort)
			0
		);

		//�A�j���[�V�����̏���
		static D3D11_SUBRESOURCE_DATA vbsd = { 0 };
		vbsd.SysMemPitch = 0;
		vbsd.SysMemSlicePitch = 0;

		vector<MODEL_VERTEX> vertex;

		uint32_t i;
		for (i = 0; i < num.vertexMax; ++i)//�|���S������
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

		//�o�[�e�b�N�X�o�b�t�@�̍X�V
		if (FAILED(DeviceManager::GetInstance().pDevice->CreateBuffer(&vbd, &vbsd, num.pVertexBuffer.GetAddressOf())))
		{
			MessageBox(0, L"�o�[�e�b�N�X�o�b�t�@�̍X�V�Ɏ��s", 0, MB_OK);
		}

		if (sMInfo.isAnim) { ++animFrame; }

		//�A�j���[�V�����t���[���̉��Z
		if (animFrame >= vsModel[sMInfo.animNum].animationMax.size() - 1)//�t���[����MAX�܂ł������烊�Z�b�g
		{
			animFrame = 0;// FlameMax[animNum] * 1.0f;
		}

		// ���_�V�F�[�_�Ƀ��f���萔�o�b�t�@��K�p����.
		DeviceManager::GetInstance().pDeviceContext->VSSetConstantBuffers(0,1,pModelCBuff.GetAddressOf());

		// �e�N�X�`����\��
		BindTexture();

		//�I�u�W�F�N�g��`��

		//�����F
		uint32_t colorKey = 0xffffffff;
		if (sMInfo.slot == SHADER_SLOT::SKELTON)
		{
			DeviceManager::GetInstance().pDeviceContext->OMSetBlendState(pBlendState.Get(), 0, colorKey);
		}
		DeviceManager::GetInstance().pDeviceContext->DrawIndexedInstanced(indexCount,1,0,0,0);
		DeviceManager::GetInstance().pDeviceContext->OMSetBlendState(0, 0, colorKey);

		// Bind�����e�N�X�`�������Z�b�g(2���ȏ�g���Ǝ��̃��f���ɉe�����ł邽��)
		ResetTextureBind();
	}

	sMInfo.isAnim = false;
}