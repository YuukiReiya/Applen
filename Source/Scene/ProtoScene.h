/**
	* @file		ProroScene.h
	* @brief	�e�X�g�p(�`�[������Ƃ��ŋ��L���Ȃ��悤�ɂ���ΌX�Ńe�X�g���₷��)
	* @author	���@����
	* @date�@	2018/02/20
*/
#pragma once
#include <string>
#include "Manager/SceneManager.h"
#include "../Sprite/Sample/SampleSprite.h"
#include "../Camera/CameraManager.h"
#include "../Fbx/TakeModelData.h"


class ProtoScene final : public Scene
{
private:
	SampleSprite * tSprite;
	umap_str_sprite uss;

	TakeModelData* tModel;
	TakeModelData* tAnim;

	std::string playerName_0;
	std::string playerName_1;

	//Debug���̂ݎg�p
	DirectX::XMFLOAT3 fEye;
	DirectX::XMFLOAT3 fLook;

	std::string path;

	void DebugMode();

public:
	ProtoScene();
	~ProtoScene();

	void Init();
	SceneBase* Update(SceneManager* root);
	void Render();
	void Destroy();
};