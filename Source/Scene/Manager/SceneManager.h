/**
	* @file		SceneManager.h
	* @brief	�V�[���Ǘ��N���X
	* @author	���@����
	* @date�@	2018/02/20
*/
#pragma once
#include "../Scene.h"
//#include "../../Input/GamePad.h"

class SceneManager final: public SceneBase
{
private:
	Scene* polymorScene;

public:
	SceneManager();
	~SceneManager() { Destroy(); };

	void Init();
	SceneBase* Update(SceneBase* scene);
	void Render();
	void Destroy();
};