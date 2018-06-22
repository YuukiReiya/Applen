/**
	* @file		SceneManager.h
	* @brief	シーン管理クラス
	* @author	東　恭亮
	* @date　	2018/02/20
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