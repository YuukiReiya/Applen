/**
	* @file		SceneBase.h
	* @brief	シーン関係のスーパークラス
	* @author	東　恭亮
	* @date　	2018/02/23
*/
#pragma once

class SceneBase
{
public:
	virtual ~SceneBase() {};

	virtual void Init() = 0;
	virtual SceneBase* Update(SceneBase* scene) = 0;
	virtual void Render() = 0;
	virtual void Destroy() = 0;
};