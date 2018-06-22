#include "ProtoScene.h"
#include "../Fbx/Manager/ModelManager.h"
#include "../Scene/Title/TitleScene.h"

using namespace std;

ProtoScene::ProtoScene()
{
	//tSprite = new TitleSprite;
	playerName_0 = "P";
	playerName_1 = "A";
}

ProtoScene::~ProtoScene()
{}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* 初期化
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void ProtoScene::Init()
{
	//tSprite->Init();
	Sprite tex;

	path = "Resource/Texture/EditorTextures.csv";
	uss = move(tex.InitTex(path.c_str()));
	//ModelManager::GetInstance().InitializeModel();

	tModel = new TakeModelData(playerName_0);
	tAnim  = new TakeModelData(playerName_1);
	tAnim->Get()->SetAnimSpeed(2);

	fEye = { 0.0f,0.0f,-10.0f };
	fLook = { 0.0f,0.0f,0.0f };
	CameraManager::GetInstance().CameraSet(fEye, fLook);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* 初期化
* @param (root) 仮想関数上の引数(実際に使いはしない)
* @return (next) 条件を満たしていれば次のシーンを返す
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
SceneBase* ProtoScene::Update(SceneManager* root)
{
	SceneBase* next = this;

	//tAnim->Get()->AnimationUpdate();

//#ifdef _DEBUG
	DebugMode();
//#endif

	return next;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* 解放
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void ProtoScene::Destroy()
{
	SAFE_DELETE(tModel);
	SAFE_DELETE(tAnim);
	ModelManager::GetInstance().Release();
	//SAFE_DELETE(tSprite);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* 描画
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void ProtoScene::Render()
{
	DeviceManager::GetInstance().SetNormalRender();

	//tModel->Render();
	//tAnim->Render();

	for (auto it : uss) {
		it.second->Render();
	}


}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* デバッグ時のみ通る
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void ProtoScene::DebugMode()
{
	static bool isKey;

	const float val = 0.1f;

	if (GetKeyState('R') & 0x80 && isKey == false)
	{
		isKey = true;
		{
			umap_str_sprite tmp;
			tmp.clear();
			uss.swap(tmp);
			Sprite tex;
			uss = move(tex.InitTex(path.c_str()));
		}

	//	MessageBoxA(NULL, "リセット", NULL, MB_OK);

		ModelManager::GetInstance().Release();
		ModelManager::GetInstance().InitializeModel();
		SAFE_DELETE(tModel);
		SAFE_DELETE(tAnim);
		tModel = new TakeModelData(playerName_0);
		tAnim = new TakeModelData(playerName_1);
		tAnim->Get()->SetAnimSpeed(2);

		fEye = { 0.0f,0.0f,-10.0f };
		fLook = { 0.0f,0.0f,0.0f };
	}
	else if (GetKeyState('W') & 0x80)
	{
		fEye.z += val;
		//fLook.z+= val;
	}
	else if (GetKeyState('S') & 0x80)
	{
		fEye.z -= val;
		fLook.z -=val;
	}
	else if (GetKeyState('A') & 0x80)
	{
		fEye.x -= 0.1f;
	}
	else if (GetKeyState('D') & 0x80)
	{
		fEye.x += 0.11f;
	}
	else if (GetKeyState('Q') & 0x80)
	{
		fLook.y += 0.01f;
	}
	else if (GetKeyState('E') & 0x80)
	{
		fLook.y -= 0.01f;
	}
	else
	{
		isKey = false;
	}

	CameraManager::GetInstance().CameraSet(fEye, fLook);
}