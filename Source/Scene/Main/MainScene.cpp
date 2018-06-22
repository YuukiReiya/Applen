#include "MainScene.h"
#include "../Title/TitleScene.h"
#include "../StageSelect/StageSelectScene.h"
#include "../../Device/DeviceManager.h"
#include "../../StageLoader/StageLoader.h"
#include "../../Input/InputManager/InputManager.h"
#include "../../Fade/FadeManager.h"
#include "../../DataManager/DataManager.h"	
#include "../../Sound/Storage/SoundStorage.h"

/*!
	@brief	名前空間
*/
using namespace DirectX;

/*!
	@def	定数宣言
*/
const int MainScene::FADE_FRAME = 45;		/*!< フェードに掛けるフレーム */

/*!
	@brief	コンストラクタ
*/
MainScene::MainScene()
{
}

/*!
	@brief	デストラクタ
*/
MainScene::~MainScene()
{
}

/*!
	@brief	初期化
*/
void MainScene::Init()
{
	/*! インスタンス生成 */
	{
		pSquareManager	= new SquaresManager;
		pStageManager	= new StageManager;
		pPlayer			= new Player;
		pBack			= new BackGround;
	}

	/*! 初期化 */
	{
		/*! プレイヤー */
		pPlayer->Initialize();

		/*! マネージャーの初期化 */
		pSquareManager->Initialize();
		pStageManager->Initialize();

		/*! ロードデータ読み込み */
		pPlayer->SetLoadData();
		pStageManager->SetLoadData();

		/*! 参照先設定 */
		pSquareManager->SetRef(pPlayer, pStageManager);
		pStageManager->SetRefData(pSquareManager);
		pPlayer->SetRef(pSquareManager);

		/*! カメラ */
		XMFLOAT3 eye, look;
		eye = StageLoader::GetInstance().GetCameraInfo().pos;
		look = eye;
		look.z = 0;
		CameraManager::GetInstance().CameraSet(eye, look);

		/*! テクスチャ */
		Sprite sprite;
		std::string path;
		path = "Resource/Texture/GameTextures.csv";
		pSprites = std::move(sprite.InitTex(path.c_str()));

		/*! 背景 */
		pBack->Init();

		/*! ステート */
		pState = new GameState(this);
	}

	/*! ステージセレクトのカーソルを現在のステージ位置に合わせる */
	int stage = StageLoader::GetInstance().GetLoadedStageNo();
	StageSelectScene::SetSelectStage(stage);

	/*! BGM再生 */
	StartBGM();

	/*! フェードイン呼び出し */
	FadeManager::GetInstance().StartFadeInOnly(FADE_FRAME);
}

/*!
	@brief	更新
*/
#include "../../Input/Keyboard/Keyboard.h"
SceneBase * MainScene::Update(SceneManager * root)
{
	/*! フェード中は更新しない */
	if (FadeManager::GetInstance().IsFade()) { return this; }

	/*! ステートの更新 */
	pState = pState->Update();


	//デバッグ
	if (Keyboard::GetButtonDown(Keyboard::SHIFT)) {
		StageLoader::GetInstance().DataLoad(StageLoader::GetInstance().GetLoadedStageNo());
		Init();
	}

	/*! 次シーンを判別 */
	SceneBase*ret = pState->next != nullptr ? pState->next : this;

	return ret;
}

/*!
	@brief	描画
*/
void MainScene::Render()
{
	/*! 1パス目:シャドウマップのレンダリングスコープ */
	{
	
	}

	/*! 2パス目:ノーマルレンダリングスコープ */
	DeviceManager::GetInstance().SetNormalRender();/*!< 描画クリア */
	{
		pBack->Render();
		pState->Render();
	}
}

/*!
	@brief	破棄
*/
void MainScene::Destroy()
{
	/*! 解放 */
	SAFE_DELETE(pState);
	SAFE_DELETE(pPlayer);
	SAFE_DELETE(pStageManager);
	SAFE_DELETE(pSquareManager);
}

/*!
	@brief	ステージに応じたBGMを再生
*/
void MainScene::StartBGM()
{
	std::string soundHash = "FLOOR";
	int no = StageLoader::GetInstance().GetLoadedStageNo();

	if (no <= 5)		{ no = 1; }
	else if (no <= 10)	{ no = 2; }
	else if (no <= 15)	{ no = 3; }
	else				{ no = 4; }
	soundHash += std::to_string(no);
	SoundStorage::GetInstance().GetWave(soundHash)->Play(true);
}

/*!  
	@brief	再生したBGMの停止
*/
void MainScene::StopBGM()
{
	std::string soundHash = "FLOOR";
	int no = StageLoader::GetInstance().GetLoadedStageNo();

	if (no <= 5)		{ no = 1; }
	else if (no <= 10)	{ no = 2; }
	else if (no <= 15)	{ no = 3; }
	else				{ no = 4; }
	soundHash += std::to_string(no);
	SoundStorage::GetInstance().GetWave(soundHash)->Stop();
}

/********************************************************/
/*			以下にゲームシーンの各ステートの定義		*/
/********************************************************/

/********************/
/*		ゲーム		*/
/********************/

/*!
	@brief	初期化
*/
void MainScene::GameState::Initialize()
{
}

/*!
	@brief	破棄
*/
void MainScene::GameState::Finalize()
{
}

/*!
	@brief	更新
*/
MainScene::StateBase * MainScene::GameState::Update()
{
	auto& player	= main->pPlayer;
	auto& stage		= main->pStageManager;
	auto& square	= main->pSquareManager;
	auto& input		= InputManager::GetInstance();
	auto& sound		= SoundStorage::GetInstance();

	/*! ポーズ */
	if (input.GetStartDown()) {
		auto pause = new PauseState(main);
		pause->Initialize();
		sound.GetWave("OPEN")->Play();
		return pause;
	}

	/*! クリア */
	if (square->GetIsClear()) {
		auto clear = new ClearState(main);
		main->StopBGM();
		sound.GetWave("CLEAR_SE")->Play();
		clear->Initialize();
		return clear;
	}

	/*! ゲーム中の更新処理 */
	player->Update();	/*!< プレイヤー */
	stage->Update();	/*!< ステージ */
	square->Update();	/*!< マス管理 */

	return this;
}

/*!
	@brief	描画
*/
void MainScene::GameState::Render()
{
	main->pPlayer->Render();
	main->pStageManager->Render();
#ifdef DebugDraw
	main->pSquareManager->Render();
#endif // DebugDraw


}

/********************/
/*		ポーズ		*/
/********************/

/*!
	@brief	初期化
*/
void MainScene::PauseState::Initialize()
{

	eMenu = PauseMenu::PAUSE_PLAY;		/*!< カーソル初期位置はプレイ */
}

/*!
	@brief	破棄
*/
void MainScene::PauseState::Finalize()
{
}

/*! 
	@brief	更新
*/
MainScene::StateBase * MainScene::PauseState::Update()
{
	/*****************************/
	/*		 ↓入力受付↓		 */
	/*****************************/
	
	/*!< メニュー決定済:true */
	const bool isDecided = (0b00010000 & eMenu) != 0;	

	/*! 決定されてれば入力は受け付けない */
	if (!isDecided) {
		Input();		/*!< 入力処理 */
		return this;
	}

	/*****************************************/
	/*		 ↓メニュー決定後の処理↓		 */
	/*****************************************/

	/*! 以降の処理はメニューが決定されたら入る */
	PauseMenu menu = static_cast<PauseMenu>(0b00001111 & eMenu);

	switch (menu)
	{
	case MainScene::PauseState::PAUSE_PLAY:
		return new GameState(main);
	case MainScene::PauseState::PAUSE_RETRY:
		main->Init();
		return new GameState(main);
	case MainScene::PauseState::PAUSE_STAGESELECT:
		next = new StageSelectScene;
		break;
	case MainScene::PauseState::PAUSE_TITLE:
		next = new TitleScene;
		break;
	default:
		assert("Invalid value");
		break;
	}

	return this;
}

/*!
	@brief	描画
*/
void MainScene::PauseState::Render()
{
	/*! プレイヤーとステージの描画は常に行う */
	main->pPlayer->Render();
	main->pStageManager->Render();

	/*! メニュー初期化 */
	auto play  = main->pSprites["pPlay_None"];
	auto retry = main->pSprites["pRetry_None"];
	auto stage = main->pSprites["pStage_None"];
	auto title = main->pSprites["pTitle_None"];

	PauseMenu menu = static_cast<PauseMenu>(0b00001111 & eMenu);

	switch (menu)
	{
	case MainScene::PauseState::PAUSE_PLAY:
		play = main->pSprites["pPlay"];
		break;
	case MainScene::PauseState::PAUSE_RETRY:
		retry= main->pSprites["pRetry"];
		break;
	case MainScene::PauseState::PAUSE_STAGESELECT:
		stage = main->pSprites["pStage"];
		break;
	case MainScene::PauseState::PAUSE_TITLE:
		title = main->pSprites["pTitle"];
		break;
	default:
		assert("Invalid value");
		break;
	}

	/*! 後ろのフェード */
	main->pSprites["pBack"]->Render();

	/*! 各メニュー */
	play->Render();
	retry->Render();
	stage->Render();
	title->Render();

}

/*!
	@brief	入力
*/
void MainScene::PauseState::Input()
{
	auto& input = InputManager::GetInstance();
	auto& sound = SoundStorage::GetInstance();

	/*****************************/
	/*		↓カーソル操作↓	 */
	/*****************************/

	/*! 左 */
	if (input.GetLeftDown()) {

		if (eMenu != PauseMenu::PAUSE_PLAY) {
			eMenu = static_cast<PauseMenu>(eMenu >> 1);
		}
		else {
			eMenu = PauseMenu::PAUSE_TITLE;
		}
		sound.GetWave("MOVE")->Play();
	}

	/*! 右 */
	if (input.GetRightDown()) {

		if (eMenu != PauseMenu::PAUSE_TITLE) {
			eMenu = static_cast<PauseMenu>(eMenu << 1);
		}
		else {
			eMenu = PauseMenu::PAUSE_PLAY;
		}
		sound.GetWave("MOVE")->Play();
	}

	/*! 決定 */
	if (input.GetDecideDown()) {
		sound.GetWave("ENTER")->Play();
		eMenu = static_cast<PauseMenu>(0b00010000 | eMenu);
	}

	/*! STARTボタンが押された */
	if (input.GetStartDown()) {
		sound.GetWave("CLOSE")->Play();
		eMenu = static_cast<PauseMenu>(0b00010000 | PauseMenu::PAUSE_PLAY);
	}

	/*****************************************/
	/*! ↓決定されてなければ処理をスキップ↓ */
	/*****************************************/
	const bool isSkip = (0b00010000 & eMenu) == 0;
	if (isSkip) { return; }/*!< 決定されてないので処理を抜ける */

	
	/*****************************************/
	/*!		↓以降は決定された瞬間の処理↓	 */
	/*****************************************/
	PauseMenu menu = static_cast<PauseMenu>(0b00001111 & eMenu);
	
	/*! "Play"を選んだ時以外はフェードをさせる  */
	if (menu != PauseMenu::PAUSE_PLAY) {
		main->StopBGM();
		FadeManager::GetInstance().StartFadeOutOnly(FADE_FRAME);
	}

}

/********************/
/*		クリア		*/
/********************/

/*!
	@def	定数宣言
*/
const int MainScene::ClearState::WAIT_FRAME = 240;/*!< このフレーム数待機してからクリアBGMを流す */

/*!
	@brief	初期化
*/
void MainScene::ClearState::Initialize()
{
	isLastStage = main->pSquareManager->GetIsLastStage();
	count = 0;

	/*! 最終ステージか判定 */
	if (isLastStage) {
		eMenu = ClearMenu::CLEAR_STAGESELECT;	/*!< 最終ステージの場合"STAGE_SELECT"と"TITLE"の二択なのでSTAGE_SELECTを入れる */
	}
	else {
		eMenu = ClearMenu::CLEAR_NEXTSTAGE;
	}
}

/*!
	@brief	破棄
*/
void MainScene::ClearState::Finalize()
{
}

/*!
	@brief	更新
*/
MainScene::StateBase * MainScene::ClearState::Update()
{
	/*****************************/
	/*		　↓SE待機↓		 */
	/*****************************/
	auto& sound = SoundStorage::GetInstance();

	if (++count == WAIT_FRAME) {
		sound.GetWave("STAGE_CLEAR")->Play(true);
	}

	/*****************************/
	/*		 ↓入力受付↓		 */
	/*****************************/

	/*! メニュー決定済:true */
	const bool isDecided = (0b00001000 & eMenu) != 0;

	/*! 決定されてれば入力は受け付けない */
	if (!isDecided) {
		Input();		/*!< 入力処理 */
		return this;
	}

	/*****************************************/
	/*		 ↓メニュー決定後の処理↓		 */
	/*****************************************/

	/*! 以降の処理はメニューが決定されたら入る */
	ClearMenu menu = static_cast<ClearMenu>(0b00000111 & eMenu);

	switch (menu)
	{
	case MainScene::ClearState::CLEAR_NEXTSTAGE:
		main->StopBGM();
		main->Init();
		return new GameState(main);
	case MainScene::ClearState::CLEAR_STAGESELECT:
		next = new StageSelectScene;
		main->StopBGM();
		sound.GetWave("STAGE_CLEAR")->Stop();
		break;
	case MainScene::ClearState::CLEAR_TITLE:
		next = new TitleScene;
		main->StopBGM();
		sound.GetWave("STAGE_CLEAR")->Stop();
		break;
	default:
		assert("Invalid value");
		break;
	}

	return this;
}

/*!
	@brief	描画
*/
void MainScene::ClearState::Render()
{
	/*! プレイヤーとステージの描画は常に行う */
	main->pPlayer->Render();
	main->pStageManager->Render();


	/*! メニュー初期化 */
	auto next  = main->pSprites["cNextStage_None"];
	auto stage = main->pSprites["cStageSelect_None"];
	auto title = main->pSprites["cTitle_None"];

	/*! 最終ステージの場合使うテクスチャが変わる */
	if (isLastStage) {

		/*!< "NEXT STAGE"が無いので呼び出した際エラーがでるようにnullptrを渡しておく */
		next = nullptr;
		/*! 読み込むテクスチャを最終ステージ用に切り替える */
		stage = main->pSprites["cLastStage_None"];
		title = main->pSprites["cLastTitle_None"];

	}

	auto menu = static_cast<ClearMenu>(0b00000111 & eMenu);

	switch (menu)
	{
	case MainScene::ClearState::CLEAR_NEXTSTAGE:
		next = main->pSprites["cNextStage"];
		break;
	case MainScene::ClearState::CLEAR_STAGESELECT:
	{
		if (isLastStage) {
			stage = main->pSprites["cLastStage"];
		}
		else {
			stage= main->pSprites["cStageSelect"];
		}
	}
		break;
	case MainScene::ClearState::CLEAR_TITLE:
	{
		if (isLastStage) {
			title = main->pSprites["cLastTitle"];
		}
		else {
			title= main->pSprites["cTitle"];
		}
	}
		break;
	default:
		assert("Invalid value");
		break;
	}

	/*! クリア文字 */
	main->pSprites["cClear"]->Render();

	/*! 各メニュー */
	if (next != nullptr) {
		next->Render();
	}
	stage->Render();
	title->Render();
}

/*!
	@brief	入力
*/
void MainScene::ClearState::Input()
{
	auto& input = InputManager::GetInstance();
	auto& sound = SoundStorage::GetInstance();

	/*****************************/
	/*		↓カーソル操作↓	 */
	/*****************************/

	/*! 左 */
	if (input.GetLeftDown()) {

		/*! 最終ステージは"NEXT STAGE"が無いので表示しない処理 */
		if (isLastStage) {

			if (eMenu == ClearMenu::CLEAR_STAGESELECT) {
				eMenu = ClearMenu::CLEAR_TITLE;
			}
			else if (eMenu == ClearMenu::CLEAR_TITLE) {
				eMenu = ClearMenu::CLEAR_STAGESELECT;
			}

		}
		/*! 最終ステージでない */
		else {
			if (eMenu != ClearMenu::CLEAR_NEXTSTAGE) {
				eMenu = static_cast<ClearMenu>(eMenu >> 1);
			}
			else {
				eMenu = ClearMenu::CLEAR_TITLE;
			}
		}
		sound.GetWave("MOVE")->Play();
	}

	/*! 右 */
	if (input.GetRightDown()) {

		/*! 最終ステージは"NEXT STAGE"が無いので表示しない処理 */
		if (isLastStage) {

			if (eMenu == ClearMenu::CLEAR_STAGESELECT) {
				eMenu = ClearMenu::CLEAR_TITLE;
			}
			else if (eMenu == ClearMenu::CLEAR_TITLE) {
				eMenu = ClearMenu::CLEAR_STAGESELECT;
			}

		}
		/*! 最終ステージでない */
		else {
			if (eMenu != ClearMenu::CLEAR_TITLE) {
				eMenu = static_cast<ClearMenu>(eMenu << 1);
			}
			else {
				eMenu = ClearMenu::CLEAR_NEXTSTAGE;
			}
		}
		sound.GetWave("MOVE")->Play();
	}

	/*! 決定 */
	if (input.GetDecideDown()) {
		eMenu = static_cast<ClearMenu>(0b00001000 | eMenu);
	}

	/*****************************************/
	/*! ↓決定されてなければ処理をスキップ↓ */
	/*****************************************/
	const bool isSkip = (0b00001000 & eMenu) == 0;
	if (isSkip) { return; }/*!< 決定されてないので処理を抜ける */


	/*****************************************/
	/*!		↓以降は決定された瞬間の処理↓	 */
	/*****************************************/
	ClearMenu menu = static_cast<ClearMenu>(0b00000111 & eMenu);

	/*! 決定音再生 */
	sound.GetWave("ENTER")->Play();
	sound.GetWave("STAGE_CLEAR")->Stop();


	/*! 次のステージのデータを読み込んでおく */
	if (menu == ClearMenu::CLEAR_NEXTSTAGE) {
		int next = StageLoader::GetInstance().GetLoadedStageNo() + 1;
		StageLoader::GetInstance().DataLoad(next);
	}

	/*! }フェードアウト命令 */
	FadeManager::GetInstance().StartFadeOutOnly(FADE_FRAME);

}