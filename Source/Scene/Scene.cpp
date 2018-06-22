#include "Scene.h"
#include "Manager/SceneManager.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* ������
* @param (scene) SceneBase����SceneManager��cast�ϊ����Ă���
* @return (Update) SceneManager�ւ���
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
SceneBase* Scene::Update(SceneBase* scene)
{
	SceneManager* root = dynamic_cast<SceneManager*>(scene);
	return Update(root);
}