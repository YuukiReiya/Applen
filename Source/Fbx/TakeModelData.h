/**
* @file		TakeModelData.h
* @brief	�ŏI�I�Ɉ����N���X
* @author	���@����
* @date�@	2018/02/20
*/
#pragma once
#include <string>
#include <memory>
#include "Manager/ModelManager.h"
#include "Render/ModelRenderBase.h"

class TakeModelData
{
private:
	std::string modelName;

public:
	TakeModelData(const std::string name) { this->modelName = name; };
	~TakeModelData() {};

	//�`��
	void Render() { ModelManager::GetInstance().Render(modelName); };

	//�Q��
	std::shared_ptr<ModelRenderBase> Get() { return ModelManager::GetInstance().GetModelData(modelName); };
};