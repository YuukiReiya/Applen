/**
* @file		TakeModelData.h
* @brief	最終的に扱うクラス
* @author	東　恭亮
* @date　	2018/02/20
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

	//描画
	void Render() { ModelManager::GetInstance().Render(modelName); };

	//参照
	std::shared_ptr<ModelRenderBase> Get() { return ModelManager::GetInstance().GetModelData(modelName); };
};