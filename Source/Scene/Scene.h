/**
	* @file		Scene.h
	* @brief	�e�V�[���Ɍp��������p�̒��p�N���X
	* @author	���@����
	* @date�@	2018/02/20
*/
#pragma once
#include "SceneBase.h"
class SceneManager;

class Scene : public SceneBase
{
public:
	virtual ~Scene() {};

	SceneBase* Update(SceneBase* scene) final;
	virtual SceneBase* Update(SceneManager* root) = 0;
};