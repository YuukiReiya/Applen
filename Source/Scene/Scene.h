/**
	* @file		Scene.h
	* @brief	各シーンに継承させる用の中継クラス
	* @author	東　恭亮
	* @date　	2018/02/20
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