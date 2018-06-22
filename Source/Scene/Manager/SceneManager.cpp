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
* �ŏ��̃V�[������
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
* �X�V
* @param (scene) ���z�֐���̈���(���ۂɎg���͂��Ȃ�)
* @return (this) SceneManager��Ԃ�
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
SceneBase* SceneManager::Update(SceneBase* scene)
{
	//�V�[���̎��s
	SceneBase* next = polymorScene->Update(this);
	FadeManager::GetInstance().Update();

	//�߂�l�����݂̃V�[���ƈقȂ��Ă���΃V�[���J��
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
* ���݂̃V�[���̕`��
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
* ���݂̃V�[����Destroy
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::Destroy()
{
	delete polymorScene;
	polymorScene = nullptr;
}