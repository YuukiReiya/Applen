/**
* @file		Model.h
* @brief	モデルを扱うクラス
* @author	東　恭亮
* @date　	2018/02/20
* @note		どこかのタイミングで1回初期化を呼べば全モデルをunorderd_mapに登録
*/
#pragma once
#include <unordered_map>
#include <string>
#include "../ModelInfo.h"
#include "../Render/ModelRenderBase.h"

class ModelManager
{
private:
	ModelManager() {};
	~ModelManager() {};

	HRESULT InitShader();
	HRESULT LoadCSVData();

	//shader
	static constexpr const wchar_t* DEFAULT_CSV_PATH = L"Resource/Model/ModelList.csv";
	static constexpr const wchar_t* DEFAULT_SHADER_PATH = L"Resource/Hlsl/Model.hlsl";
	static constexpr const wchar_t* SKELTON_SHADER_PATH = L"Resource/Hlsl/AlphaModel.hlsl";
	std::vector<modelstruct::MODEL_SHADER*> sMShader;

	//このコンテナに全てのモデルを格納
	std::unordered_map<std::string, std::shared_ptr<ModelRenderBase>> pMRender;

public:
	static ModelManager & GetInstance()
	{
		static ModelManager am;
		return am;
	}

	void InitializeModel();
	void Render(const std::string name);

	//必ず最後に呼び出すこと
	void Release();

	/////各種インライン/////
	//inlineは省略
	std::shared_ptr<ModelRenderBase> GetModelData(const std::string name) { return this->pMRender[name]; }
};