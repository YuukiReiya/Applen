/**
* @file		AnimModelrender.h
* @brief	モデルデータの読み込み
* @author	東　恭亮
* @date　	2018/02/20
*/
#pragma once
#include "ModelRenderBase.h"

class AnimModelRender : public ModelRenderBase
{
private:
	HRESULT InitializeResource(const std::string fileName);

	modelstruct::MODEL_SHADER_CBUFF			sMCBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	pModelCBuff;
	uint32_t								animFrame;
	uint32_t								indexCount;

public:
	AnimModelRender();
	~AnimModelRender() {};

	void Init(const std::string modelPath)final;
	void Render()final;
};