/**
	* @file		SceneBase.h
	* @brief	�V�[���֌W�̃X�[�p�[�N���X
	* @author	���@����
	* @date�@	2018/02/23
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