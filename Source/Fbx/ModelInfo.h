/**
* @file		ModelInfo.h
* @brief	Model�Ɏg���\���̂̐錾
* @author	���@����
* @date�@	2018/02/20
*/
#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <windows.h>
#include <wrl.h>
#include <d3d11.h>

#include "../Device/DeviceManager.h"

namespace modelstruct
{
	enum SHADER_SLOT : int
	{
		DEFAULT = 0,
		SKELTON = 1
	};

	struct MODEL_SHADER
	{
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	pPixcelShader;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	pVertexShader;
		//Microsoft::WRL::ComPtr<ID3D11GeometryShader>	pGeometryShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>		pInputLayout;
	};

	struct MODEL_SHADER_CBUFF
	{
		DirectX::XMMATRIX xmWorld;
		float fAlpha;
	};

	struct MODEL_INFO
	{
		DirectX::XMFLOAT3 xfModelPos;	//�ʒu
		DirectX::XMFLOAT3 xfModelScale;	//�傫��
		DirectX::XMFLOAT3 xfModelRot;	//��]
		DirectX::XMMATRIX xmLoacalMat;	//���[�J���}�g���N�X
		uint32_t animNum;				//�A�j���[�V�����ԍ�
		uint32_t texCount;				//�e�N�X�`���J�E���g
		uint32_t nodeMax;				//mesh��
		bool isAnim;					//�A�j���[�V����
		int  slot;						//shader�X���b�g
		float fAlpha;

		MODEL_INFO() { SecureZeroMemory(this, sizeof(this)); };
		~MODEL_INFO() {};
	};

	struct MODEL_VERTEX
	{
		DirectX::XMFLOAT3 xfPos;
		DirectX::XMFLOAT3 xfNorm;
		DirectX::XMFLOAT2 xfTex;
		
	};

	struct MODEL_MATERIAL
	{
		DirectX::XMFLOAT4 xfDiffuse;
		DirectX::XMFLOAT3 xfSpecular;
		DirectX::XMFLOAT3 xfAmbient;
	};

	struct MODEL_MESH
	{
		std::vector<modelstruct::MODEL_VERTEX>						vertex;
		std::vector<modelstruct::MODEL_MATERIAL>					material;
		std::vector<std::vector<std::vector<DirectX::XMFLOAT3>>>	animation;
		Microsoft::WRL::ComPtr<ID3D11Buffer>						pVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>						pIndexBuffer;
		std::string													textureName;
		DirectX::XMMATRIX											mat;
		uint32_t													vertexMax;
		std::vector<uint32_t>										animationMax;

		MODEL_MESH() { SecureZeroMemory(this, sizeof(this)); };
		~MODEL_MESH() 
		{
			SAFE_DELETE_CONTAINER(vertex);
			SAFE_DELETE_CONTAINER(material);
			SAFE_DELETE_CONTAINER(animation);
			SAFE_DELETE_CONTAINER(animationMax);
		};
	};
}