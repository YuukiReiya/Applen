#include "StaticModelRender.h"
#include "../Load//ModelLoad.h"
#include "../../Device/DeviceManager.h"

using namespace modelstruct;
using namespace DirectX;
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* �f�o�C�X�̏������ƃ��\�[�X�̓ǂݍ���
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void StaticModelRender::Init(const std::string filePath)
{
	//���\�[�X
	InitializeResource(filePath);

	//�e�N�X�`���T���v���[�̏�����
	InitSampler();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* ���\�[�X�̒ǉ�
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT StaticModelRender::InitializeResource(const std::string filePath)
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

	//���b�V���T�C�Y�̕ۑ�
	sMInfo.nodeMax = (unsigned)vsModel.size();

	D3D11_SUBRESOURCE_DATA  vbsd;
	SecureZeroMemory(&vbsd, sizeof(D3D11_SUBRESOURCE_DATA));

	//���b�V���̐��������[�v
	for (auto &num : vsModel)
	{
		//���b�V�����̍ő咸�_��
		num.vertexMax = (unsigned)num.vertex.size();

		vector<modelstruct::MODEL_VERTEX> vertex;

		//�T�u�\�[�X�̍쐬
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

		//�o�[�e�b�N�X�o�b�t�@�p�f�X�N�̍쐬
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
		SecureZeroMemory(&ibsd, sizeof(D3D11_SUBRESOURCE_DATA));
		ibsd.pSysMem = ind.data();

		//�C���f�b�N�X�o�b�t�@�p�f�X�N�̍쐬
		CD3D11_BUFFER_DESC ibd(sizeof(unsigned short) * indeSize, D3D11_BIND_INDEX_BUFFER);

		//�C���f�b�N�X�o�b�t�@�̍쐬
		if (FAILED(DeviceManager::GetInstance().pDevice->CreateBuffer(&ibd, &ibsd, num.pIndexBuffer.GetAddressOf())))
		{
			MessageBox(0, L"�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s", 0, MB_OK);
		}
	}
	
	//�R���X�^���g�o�b�t�@�p�f�X�N�̍쐬
	D3D11_BUFFER_DESC cb;
	SecureZeroMemory(&cb, sizeof(cb));
	cb.ByteWidth = sizeof(MODEL_SHADER_CBUFF);
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//�R���X�^���g�o�b�t�@�̍쐬
	if(FAILED(DeviceManager::GetInstance().pDevice->CreateBuffer(&cb,nullptr,&pModelCBuff)))
	{
		MessageBox(0, L"�R���X�^���g�o�b�t�@�쐬���s", 0, MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* ���f���`��
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void StaticModelRender::Render()
{
	// �s��ϊ�
	MatrixConvert();

	//Mesh�̐��Ԃ�`��
	for (auto &num : vsModel)
	{
		// �o�[�e�b�N�X�o�b�t�@���Z�b�g
		const UINT stride = sizeof(MODEL_VERTEX);
		const UINT offset = 0;
		DeviceManager::GetInstance().pDeviceContext->IASetVertexBuffers(
			0,
			1,
			num.pVertexBuffer.GetAddressOf(),
			&stride,
			&offset
		);

		//�C���f�b�N�X�o�b�t�@���Z�b�g
		DeviceManager::GetInstance().pDeviceContext->IASetIndexBuffer(
			num.pIndexBuffer.Get(),
			DXGI_FORMAT_R16_UINT, // �e�C���f�b�N�X��1��16�r�b�g�����Ȃ������ishort)
			0
		);

		D3D11_MAPPED_SUBRESOURCE pData;
		SecureZeroMemory(&pData, sizeof(pData));

		MODEL_SHADER_CBUFF sg;
		SecureZeroMemory(&sg, sizeof(sg));

		//map/unmap
		if (SUCCEEDED(DeviceManager::GetInstance().pDeviceContext->Map(pModelCBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			//���[���h�s��
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
			MessageBox(0,L"�T�u���\�[�X�f�[�^��ǂݍ��߂܂���ł���",0, MB_OK);
		}

		// ���_�V�F�[�_�Ƀ��f���萔�o�b�t�@��K�p����.
		DeviceManager::GetInstance().pDeviceContext->VSSetConstantBuffers(0,1,&pModelCBuff);

		// �e�N�X�`����\��
		BindTexture();

		//�I�u�W�F�N�g��`��
		uint32_t colorKey = 0xffffffff;
		if(sMInfo.slot == SHADER_SLOT::SKELTON)
		{
			DeviceManager::GetInstance().pDeviceContext->OMSetBlendState(pBlendState.Get(), 0, colorKey);
		}
		DeviceManager::GetInstance().pDeviceContext->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);
		DeviceManager::GetInstance().pDeviceContext->OMSetBlendState(0, 0, colorKey);

		// Bind�����e�N�X�`�������Z�b�g(2���ȏ�g���Ǝ��̃��f���ɉe�����ł邽��)
		ResetTextureBind();
	}
}