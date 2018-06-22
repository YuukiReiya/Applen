/**
* @file		ModelRenderBase.h
* @brief	モデル描画のベースクラス
* @author	東　恭亮
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

	//継承先で実装
	virtual void Init(const std::string modelPath) = 0;
	virtual void Render() = 0;

	//初期ロードuv(現時点では1つのuvテクスチャのみ対応)
	HRESULT LoadTexture(const wchar_t* texPath);

	//途中変更uv
	void SetTexture(ID3D11ShaderResourceView* usetex);

	/////各種インライン関数/////
	//inline修飾子は省略
	//PRSの代入
	void SetModelPos(const DirectX::XMFLOAT3 pos) { this->sMInfo.xfModelPos = pos; };
	void SetModelRot(const DirectX::XMFLOAT3 rot) { this->sMInfo.xfModelRot = rot; };
	void SetModelScale(const DirectX::XMFLOAT3 rot) { this->sMInfo.xfModelScale = rot; };

	//PRSを参照
	DirectX::XMFLOAT3 GetModelPos()const { return this->sMInfo.xfModelPos; };
	DirectX::XMFLOAT3 GetModelRot()const { return this->sMInfo.xfModelRot; };
	DirectX::XMFLOAT3 GetModelScale()const { return this->sMInfo.xfModelScale; };

	// 透過度
	void SetSkeltonValue(const float alpha) 
	{ 
		this->sMInfo.fAlpha = alpha;
	};

	//shaderスロット
	void ShaderSlot(const int slot) { this->sMInfo.slot = slot; };
	int GetShaderSlot()const { return this->sMInfo.slot; };

	//アニメーションの更新(呼ばないと止まる)
	void AnimationUpdate() { this->sMInfo.isAnim = true; };

	//アニメーションの速度を変更
	void SetAnimSpeed(const uint32_t speed) { this->animSpeed = speed; };
};