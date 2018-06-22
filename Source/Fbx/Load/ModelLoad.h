/**
* @file		ModelLoad.h
* @brief	モデルデータの読み込み
* @author	東　恭亮
* @date　	2018/02/20
*/
#pragma once
#include "../ModelInfo.h"

class ModelLoad
{
private:
	//最終的に渡す構造体
	std::vector<modelstruct::MODEL_MESH> mesh;
	bool isLoadComplete;

	static constexpr uint32_t FILE_BYTE = 4;

public:
	ModelLoad() {};
	~ModelLoad() {};

	//読み込み
	HRESULT Load(const std::string path);

	//インライン
	std::vector<modelstruct::MODEL_MESH> GetMesh() { return this->mesh; };

	//bool Comp() { return this->isLoadComplete; };
};