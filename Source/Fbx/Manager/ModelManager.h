/**
* @file		Model.h
* @brief	���f���������N���X
* @author	���@����
* @date�@	2018/02/20
* @note		�ǂ����̃^�C�~���O��1�񏉊������ĂׂΑS���f����unorderd_map�ɓo�^
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

	//���̃R���e�i�ɑS�Ẵ��f�����i�[
	std::unordered_map<std::string, std::shared_ptr<ModelRenderBase>> pMRender;

public:
	static ModelManager & GetInstance()
	{
		static ModelManager am;
		return am;
	}

	void InitializeModel();
	void Render(const std::string name);

	//�K���Ō�ɌĂяo������
	void Release();

	/////�e��C�����C��/////
	//inline�͏ȗ�
	std::shared_ptr<ModelRenderBase> GetModelData(const std::string name) { return this->pMRender[name]; }
};