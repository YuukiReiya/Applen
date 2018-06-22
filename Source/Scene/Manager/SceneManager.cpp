#include "SceneManager.h"
#include "../../Fade/FadeManager.h"
#include "../ProtoScene.h"
#include "../../Scene/Title/TitleScene.h"
#include "../../Scene/StageSelect/StageSelectScene.h"
#include "../../Scene/Main/MainScene.h"
#include "../../StageLoader/StageLoader.h"


SceneManager::SceneManager()
	:polymorScene(nullptr)
{}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* 最初のシーンを代入
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::Init()
{
	//polymorScene = new ProtoScene;
	polymorScene = new TitleScene;
	//polymorScene = new StageSelectScene;

	{
	/*	int no = 1;
		std::string tmp = std::to_string(no);
		if (!StageLoader::GetInstance().DataLoad(no)) { MessageBoxA(NULL, "w", NULL, MB_OK); }
		polymorScene = new MainScene;*/
	}
	polymorScene->Init();

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* 更新
* @param (scene) 仮想関数上の引数(実際に使いはしない)
* @return (this) SceneManagerを返す
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
SceneBase* SceneManager::Update(SceneBase* scene)
{
	//シーンの実行
	SceneBase* next = polymorScene->Update(this);
	FadeManager::GetInstance().Update();

	//戻り値が現在のシーンと異なっていればシーン遷移
	if (next != polymorScene)
	{
		Scene* casted = dynamic_cast<Scene*>(next);
		polymorScene->Destroy();
		delete polymorScene;

		polymorScene = casted;
		polymorScene->Init();
	}

	return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* 現在のシーンの描画
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::Render()
{
	polymorScene->Render();
	FadeManager::GetInstance().Render();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* 現在のシーンのDestroy
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::Destroy()
{
	delete polymorScene;
	polymorScene = nullptr;
}