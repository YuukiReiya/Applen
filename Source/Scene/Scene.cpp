#include "Scene.h"
#include "Manager/SceneManager.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* ‰Šú‰»
* @param (scene) SceneBase‚©‚çSceneManager‚Écast•ÏŠ·‚µ‚Ä‚¢‚é
* @return (Update) SceneManager‚Ö‚¢‚­
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
SceneBase* Scene::Update(SceneBase* scene)
{
	SceneManager* root = dynamic_cast<SceneManager*>(scene);
	return Update(root);
}