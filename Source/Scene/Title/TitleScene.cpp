#include "TitleScene.h"
#include "../../Device/DeviceManager.h"
#include "../../Input/InputManager/InputManager.h"
#include "../../Fade/FadeManager.h"
#include "../StageSelect/StageSelectScene.h"
#include "../../DataManager/DataManager.h"
#include "../../Sound/Storage/SoundStorage.h"


/*!
	@def	定数宣言
*/
const int TitleScene::FADE_FRAME = 45;/*!< フェードさせるフレーム */

/*!
	@brief	名前空間
*/
using namespace DirectX;
using namespace std;

/*!
	@brief	コンストラクタ
*/
TitleScene::TitleScene()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* 初期化
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void TitleScene::Init()
{
	/*! テクスチャ初期化 */
	Sprite textures;
	std::string path = "Resource/Texture/TitleTextures.csv";
	pSprites = std::move(textures.InitTex(path.c_str()));

	pTbg = new TitleBackGround;
	pTbg->Init();
		
	/*! パラメータ初期化 */
	DataManager::GetInstance().Load();					/*!< データ読み込み */
	FadeManager::GetInstance().StartFadeInOnly(FADE_FRAME);		/*!< フェード開始 */
	
	eMenu = TitleMenu::TITLE_PLAY;						/*!< 選択メニュー */
	pState = new PlayState(this);						/*!< 選択ステート */

	/*! カメラ設定 */
	XMFLOAT3 fEye	= { 0,0,-10.0f };
	CameraManager::GetInstance().CameraSet(fEye);

	/*! BGM */
	auto bgm = SoundStorage::GetInstance().GetWave("TITLE");
	bgm->Play(true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* 更新
* @param (root) 仮想関数上の引数(実際に使いはしない)
* @return (next) 条件を満たしていれば次のシーンを返す
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
SceneBase* TitleScene::Update(SceneManager* root)
{
	/*! フェード中は更新しない */
	if (FadeManager::GetInstance().IsFade()) { return this; }

	/*! メニュー決定済:true */
	const bool isDecided = (0b1000 & eMenu) != 0;

	/*! 決定されてれば入力は受け付けない */
	if (!isDecided) {
		Input();
		return this;
	}

	/*! 決定されたステートの更新 */
	pState->Update();

	/*! 次シーンを判別 */
	SceneBase* ret = pState->next != nullptr ? pState->next : this;

	return ret;

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* 描画
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void TitleScene::Render()
{
	DeviceManager::GetInstance().SetNormalRender();
	auto menu = static_cast<TitleMenu>(0b0111 & eMenu);

	auto play		= pSprites["Play_None"];
	auto credit		= pSprites["Credit_None"];
	auto exit		= pSprites["Exit_None"];

	pTbg->Render();

	/*! ロゴ */
	pSprites["Logo"]->Render();

	/*! 選択中メニューのスプライトを変える */
	switch (menu)
	{
	case TitleScene::TITLE_PLAY:
		play = pSprites["Play"];
		break;
	case TitleScene::TITLE_CREDIT:
		credit = pSprites["Credit"];
		break;
	case TitleScene::TITLE_EXIT:
		exit = pSprites["Exit"];
		break;
	default:
		assert("Invalid value");
		break;
	}


	play->Render();
	credit->Render();
	exit->Render();


	if (menu == TitleMenu::TITLE_CREDIT) {
		pState->Render();
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* 解放
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void TitleScene::Destroy()
{
}

/*! 
	@brief	入力
*/

void TitleScene::Input()
{
	auto& input = InputManager::GetInstance();
	auto& sound = SoundStorage::GetInstance();

	/*****************************/
	/*		↓カーソル操作↓	 */
	/*****************************/

	/*! 上 */
	if (input.GetUpDown()) {

		if (eMenu != TitleMenu::TITLE_PLAY) {
			eMenu = static_cast<TitleMenu>(eMenu >> 1);
		}
		else {
			eMenu = TitleMenu::TITLE_EXIT;
		}
		sound.GetWave("MOVE")->Play();
	}

	/*! 下 */
	if (input.GetUnderDown()) {

		if (eMenu != TitleMenu::TITLE_EXIT) {
			eMenu = static_cast<TitleMenu>(eMenu << 1);
		}
		else {
			eMenu = TitleMenu::TITLE_PLAY;
		}
		sound.GetWave("MOVE")->Play();
	}

	/*! 決定 */
	if (input.GetDecideDown()) {
		eMenu = static_cast<TitleMenu>(0b1000 | eMenu);
		sound.GetWave("ENTER")->Play();
	}

	/*! メニュー決定済:true */
	const bool isDecided = (0b1000 & eMenu) != 0;

	/*! 決定されてれば処理をスキップ */
	if (!isDecided) { return; }

	/*! 決定された瞬間の処理 */

	auto menu = static_cast<TitleMenu>(0b0111 & eMenu);

	auto& fade = FadeManager::GetInstance();	

	switch (menu)
	{
		/*! ゲームプレイ */
	case TitleScene::TITLE_PLAY:
	{
		fade.StartFadeOutOnly(FADE_FRAME);
		pState = new PlayState(this);
		StageSelectScene::SetSelectStage(1);/*!< 選択ステージを1番目にする */
	}
		break;
		/*! クレジット */
	case TitleScene::TITLE_CREDIT:
	{
		pState = new CreditState(this);
		pState->Initialize();
	}
		break;
		/*! ゲーム終了 */
	case TitleScene::TITLE_EXIT:
	{
		fade.StartFadeOutOnly(FADE_FRAME);
		pState = new ExitState(this);
	}
		break;
	default:
		assert("Invalid value");
		break;
	}


}

/********************************************************/
/*			以下にタイトルシーンの各ステートの定義		*/
/********************************************************/

/********************/
/*		"PLAY"		*/
/********************/

/*!
	@brief	初期化
*/
void TitleScene::PlayState::Initialize()
{
}

/*!
	@brief	破棄
*/
void TitleScene::PlayState::Finalize()
{
}

/*!
	@brief	更新
*/
void TitleScene::PlayState::Update()
{
	auto bgm = SoundStorage::GetInstance().GetWave("TITLE");
	bgm->Stop();	/*!< BGM停止 */

	next = new StageSelectScene;
}

/*!
	@brief	描画
*/
void TitleScene::PlayState::Render()
{
}

/********************/
/*	   "CREDIT"  	*/
/********************/

/*!
	@brief	定数宣言
*/
const string TitleScene::CreditState::HASH_NAME = "CreditURL";	/*!< 表示するテクスチャのハッシュ名 */
const int TitleScene::CreditState::SCALE_FRAME = 50;			/*!< 拡大縮小にかけるフレーム */

/*! スケール変化に伴い変化する座標のオフセット */
const XMFLOAT2 TitleScene::CreditState::POSITION_OFFSET = {
	-2.0f / SCALE_FRAME,	/*!< x */
	-1.0f / SCALE_FRAME,	/*!< y */
};

/*! テクスチャのスケール */
const XMFLOAT2 TitleScene::CreditState::TEXTURE_SCALE = {
	0.2f,		/*!< x */
	0.2f,		/*!< y */
};


/*!
	@brief	初期化
*/

void TitleScene::CreditState::Initialize()
{
	/*! テクスチャのポジションとスケールをゼロにする */
	scene->pSprites[HASH_NAME]->GetTexData()->xfPos ={ 0, 0, -6 };
	scene->pSprites[HASH_NAME]->SetScale({ 0,0 });
	/*! カウンタをリセット */
	count = 0;
	isOpen = true;/*!< 初期はクレジットを開く状態 */
}

/*!
	@brief	破棄
*/
void TitleScene::CreditState::Finalize()
{
}

/*!
	@brief	更新
*/
void TitleScene::CreditState::Update()
{
	/*! フレームをカウント */
	++count;

	/*! スケーリング */
	if (count <= SCALE_FRAME) {
		Scaling();
	}
	else {

		/*! スケーリング開始命令 */
		if (InputManager::GetInstance().GetDecideDown()) {
			count = 0;
			SoundStorage::GetInstance().GetWave("ENTER")->Play();
		}
	}

	/*! スケーリングが終わった瞬間 */
	if (count == SCALE_FRAME) {

		/*! 閉じ終わったらメニューの入力済みを取り消し */
		if (!isOpen) {
			scene->eMenu = static_cast<TitleMenu>(0b0111 & scene->eMenu);
		}

		isOpen = !isOpen;/*!< 逆の状態が起こるようにする */
	}
}

/*!
	@brief	描画
*/
void TitleScene::CreditState::Render()
{
	scene->pSprites[HASH_NAME]->Render();
}

/*!
	@brief	スケーリング処理を行う
*/
void TitleScene::CreditState::Scaling()
{
	/*! スケールの加算量 */
	XMFLOAT2 add = {
		TEXTURE_SCALE.x / SCALE_FRAME,
		TEXTURE_SCALE.y / SCALE_FRAME,
	};

	auto sprite = scene->pSprites[HASH_NAME];
	auto texture = scene->pSprites[HASH_NAME]->GetTexData();

	/*! 符号を判定する */
	int sign = isOpen ? 1 : -1;		/*!< OPEN:1,CLOSE:-1 */

	/*! スケールの変換処理 */
	XMFLOAT2 scale = sprite->GetScale();
	scale.x += add.x*sign;
	scale.y += add.y*sign;

	sprite->SetScale(scale);

	/*! 座標を補正する */
	XMFLOAT2 pos = sprite->GetPos2D();
	pos.x += POSITION_OFFSET.x*sign;
	pos.y += POSITION_OFFSET.y*sign;

	sprite->SetPos(pos);

}

/********************/
/*		"EXIT"		*/
/********************/

/*!
	@brief	初期化
*/
void TitleScene::ExitState::Initialize()
{
}

/*!
	@brief	破棄
*/
void TitleScene::ExitState::Finalize()
{
}

/*!
	@brief	更新
*/
void TitleScene::ExitState::Update()
{
	auto bgm = SoundStorage::GetInstance().GetWave("TITLE");
	bgm->Stop();	/*!< BGM停止 */

	exit(NULL);
}

/*!
	@brief	描画
*/
void TitleScene::ExitState::Render()
{
}
