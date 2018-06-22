/**
* @file		StaticModelRender.h
* @brief	�A�j���[�V�����������f���̕`��
* @author	���@����
* @date 	2018/02/20
*/
#pragma once
#include "ModelRenderBase.h"

class StaticModelRender : public ModelRenderBase
{
private:
	HRESULT InitializeResource(const std::string filePath);

	ID3D11Buffer* pModelCBuff;
	uint32_t	  indexCount;

public:
	StaticModelRender() { indexCount = 0; };
	~StaticModelRender() 
	{ 
		pModelCBuff->Release();
		pModelCBuff = nullptr;
	};

	void Init(const std::string modelPath)final;
	void Render()final;
};