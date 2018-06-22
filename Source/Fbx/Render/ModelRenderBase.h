/**
* @file		ModelRenderBase.h
* @brief	���f���`��̃x�[�X�N���X
* @author	���@����
* @date 	2018/02/20
*/
#pragma once
#include "../ModelInfo.h"
#include <memory>

class ModelRenderBase
{
private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			pSamplerState;
	ID3D11ShaderResourceView*	pTexture;

	uint32_t texCounter;
	uint32_t animSpeed;

protected:
	HRESULT InitSampler();
	void BindTexture();
	void ResetTextureBind();
	void MatrixConvert();

	modelstruct::MODEL_INFO					sMInfo;
	std::vector<modelstruct::MODEL_MESH>	vsModel;
	DirectX::XMMATRIX						modelTransform;
	Microsoft::WRL::ComPtr<ID3D11BlendState>pBlendState;

public:
	ModelRenderBase();
	~ModelRenderBase() {};

	//�p����Ŏ���
	virtual void Init(const std::string modelPath) = 0;
	virtual void Render() = 0;

	//�������[�huv(�����_�ł�1��uv�e�N�X�`���̂ݑΉ�)
	HRESULT LoadTexture(const wchar_t* texPath);

	//�r���ύXuv
	void SetTexture(ID3D11ShaderResourceView* usetex);

	/////�e��C�����C���֐�/////
	//inline�C���q�͏ȗ�
	//PRS�̑��
	void SetModelPos(const DirectX::XMFLOAT3 pos) { this->sMInfo.xfModelPos = pos; };
	void SetModelRot(const DirectX::XMFLOAT3 rot) { this->sMInfo.xfModelRot = rot; };
	void SetModelScale(const DirectX::XMFLOAT3 rot) { this->sMInfo.xfModelScale = rot; };

	//PRS���Q��
	DirectX::XMFLOAT3 GetModelPos()const { return this->sMInfo.xfModelPos; };
	DirectX::XMFLOAT3 GetModelRot()const { return this->sMInfo.xfModelRot; };
	DirectX::XMFLOAT3 GetModelScale()const { return this->sMInfo.xfModelScale; };

	// ���ߓx
	void SetSkeltonValue(const float alpha) 
	{ 
		this->sMInfo.fAlpha = alpha;
	};

	//shader�X���b�g
	void ShaderSlot(const int slot) { this->sMInfo.slot = slot; };
	int GetShaderSlot()const { return this->sMInfo.slot; };

	//�A�j���[�V�����̍X�V(�Ă΂Ȃ��Ǝ~�܂�)
	void AnimationUpdate() { this->sMInfo.isAnim = true; };

	//�A�j���[�V�����̑��x��ύX
	void SetAnimSpeed(const uint32_t speed) { this->animSpeed = speed; };
};