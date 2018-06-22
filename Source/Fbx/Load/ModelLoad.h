/**
* @file		ModelLoad.h
* @brief	���f���f�[�^�̓ǂݍ���
* @author	���@����
* @date�@	2018/02/20
*/
#pragma once
#include "../ModelInfo.h"

class ModelLoad
{
private:
	//�ŏI�I�ɓn���\����
	std::vector<modelstruct::MODEL_MESH> mesh;
	bool isLoadComplete;

	static constexpr uint32_t FILE_BYTE = 4;

public:
	ModelLoad() {};
	~ModelLoad() {};

	//�ǂݍ���
	HRESULT Load(const std::string path);

	//�C�����C��
	std::vector<modelstruct::MODEL_MESH> GetMesh() { return this->mesh; };

	//bool Comp() { return this->isLoadComplete; };
};