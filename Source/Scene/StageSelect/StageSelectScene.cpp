#include "StageSelectScene.h"
#include "../../Fade/FadeManager.h"
#include "../../Camera/CameraManager.h"
#include "../../Input/InputManager/InputManager.h"
#include "../Title/TitleScene.h"
#include "../Main/MainScene.h"
#include "../../StageLoader/StageLoader.h"
#include <math.h>
#include "../../Sound/Storage/SoundStorage.h"

/*! 
	@brief	名前空間
*/
using namespace DirectX;

/*!
	@brief	定数宣言
*/
const int StageSelectScene::FADE_FRAME = 45;				/*!< フェードさせるフレーム */
const int StageSelectScene::STAGE_WIDTH_NUM = 5;			/*!< 横1段に並ぶステージの数 */
const int StageSelectScene::STAGE_HEIGHT_NUM = 4;			/*!< ステージの段数 */
const int StageSelectScene::CAMERA_MOVE_FRAME = 30;			/*!< カメラを移動させるフレーム */
const float StageSelectScene::SELECT_STAGE_UP_VALUE = 0.8f;	/*!< 選択中のステージの上昇量 */

/*! カメラの移動オフセット */
const XMFLOAT3 StageSelectScene::CAMERA_MOVE_OFFSET = {
	0.0f,		/*! x */
	4.0f,		/*! y */
	0.0f,		/*! z */
};

/*!
	@brief	静的メンバ変数の宣言
*/
int StageSelectScene::selectNo = 1;/*!< 初期値は1とし、ステージ1を選択状態にする */

/*!
	@brief	コンストラクタ
*/
StageSelectScene::StageSelectScene()
{
}

/*!
	@brief	デストラクタ
*/
StageSelectScene::~StageSelectScene()
{
}

/*!
	@brief	初期化
*/
void StageSelectScene::Init()
{
	/*! オブジェクトの取得 */
	pSideObj = std::make_shared<TakeModelData>("ssSide");
	pMiddleObj = std::make_shared<TakeModelData>("ssMiddle");

	/*! オブジェクトの張替え用テクスチャ */
	pTexesTake = new TexesTake;
	pTexesTake->TexesCSVLoad("Resource/Model/uvStage/LoadUVPath.csv");

	/*! パラメータ初期化 */
	eState = SelectState::SELECT_STAGE;/*!< ステージを選択状態に */

	/*! ループ回数 */
	const int NUM = STAGE_HEIGHT_NUM * STAGE_WIDTH_NUM;/*!< オブジェクトの数 */

	/*! オブジェクトの表示座標の算出 */
	objPos.clear();
	for (int i = 0; i < NUM; ++i) {

		const int x = i % STAGE_WIDTH_NUM;
		const int y = i / STAGE_WIDTH_NUM;

		/*!  */
		XMFLOAT3 pos;
		/*! (x,y)=(0,0)座標＋オフセット＊番号 */
		pos.x = -4 + 1.8f * static_cast<float>(x);
		pos.y = -0 + 3.6f * static_cast<float>(y);
		pos.z = 1;

		objPos.push_back(pos);
	}

	/*! テクスチャ初期化 */
	Sprite load;
	auto sprites = load.InitTex("Resource/Texture/StageSelectTextures.csv");
	pSprites = std::move(sprites);	/*!< テクスャリストの登録 */

	/*! カメラ設定 */
	const int height = (selectNo - 1) / STAGE_WIDTH_NUM;	/*!< 高さ */
	XMFLOAT3 fLook;/*!< 注視点 */

	/*! 視点位置算出 */
	fEyeFrom = {
		  0.0f + CAMERA_MOVE_OFFSET.x,
		  0.0f + height * CAMERA_MOVE_OFFSET.y,
		-10.0f + CAMERA_MOVE_OFFSET.z,
	};

	fLook = fEyeFrom;
	fLook.z = 0;		/*!< 補正 */
	fEyeTo = fEyeFrom;	/*!< 移動先座標の初期化 */
	CameraManager::GetInstance().CameraSet(fEyeFrom, fLook);

	/*! BGM */
	auto bgm = SoundStorage::GetInstance().GetWave("STAGE_SELECT");
	bgm->Play(true);

	/*! フェードイン呼び出し */
	FadeManager::GetInstance().StartFadeInOnly(FADE_FRAME);
}

/*!
	@brief	更新
*/
SceneBase * StageSelectScene::Update(SceneManager * root)
{
	/*! フェード中は処理しない */
	auto& fade = FadeManager::GetInstance();
	if (fade.IsFade()) { return this; }

	/*! カメラの座標と移動先を比較して一致していなければ移動中と判定 */
	const bool misMatchX = fEyeFrom.x != fEyeTo.x;
	const bool misMatchY = fEyeFrom.y != fEyeTo.y;
	const bool misMatchZ = fEyeFrom.z != fEyeTo.z;
	const bool isMoving = misMatchX || misMatchY || misMatchZ;

	/*! カメラが移動している */
	if (isMoving) {
		CameraMove();	/*!< カメラ移動処理 */
		return this;
	}

	const bool canInput = (0b100 & eState) == 0;/*!< 選択可能か判定 */

	/*! 選択済みなら入力を受け付けない */
	if (canInput) {
		Input();
	}
	/*! 選択済み処理 */
	else {

		/*! BGM停止 */
		auto& sound = SoundStorage::GetInstance();
		sound.GetWave("STAGE_SELECT")->Stop();

		auto state = static_cast<SelectState>(0b11 & eState);
		switch (state)
		{
		case StageSelectScene::SELECT_STAGE:
			StageLoader::GetInstance().DataLoad(selectNo);
			return new MainScene;
		case StageSelectScene::RETURN_TTILE:
			return new TitleScene;
		}
	}
	return this;
}

/*!
	@brief	描画
*/
void StageSelectScene::Render()
{
	/*! 1パス目:シャドウマップのレンダリングスコープ */
	{

	}

	/*! 2パス目:ノーマルレンダリングスコープ */
	DeviceManager::GetInstance().SetNormalRender();/*!< 描画クリア */
	{
		/*! ブロック数 */
		const int NUM = STAGE_HEIGHT_NUM * STAGE_WIDTH_NUM;

		/*! タイトル選択時 */
		const bool isTitle = (0b10 & eState) != 0;

		/************************************/
		/*		 非選択ステージの描画		*/
		/************************************/

		/*! ステージ左端( 1 , 6 , 11 , 16 ) */
		for (int i = 1; i <= NUM; i += STAGE_WIDTH_NUM) {

			/*! 選択ステージ */
			const bool isSelect = selectNo == i;	

			/*! 選択ステージは描画しない */
			const bool isSkip = isSelect && !isTitle;
			if (isSkip) { continue; }

			/*! 座標とテクスチャが入っている可変長配列のインデックス */
			const int index = i - 1;
			/*! 描画 */
			pSideObj->Get()->SetModelPos(objPos[index]);
			pSideObj->Get()->SetTexture(pTexesTake->GetTex(index));
			pSideObj->Render();
		}

		/*! ステージ中央群( 2～4 , 7～9 , 12～14 , 17～19 ) */
		for (int i = 2; i <= NUM; ++i) {

			/*! 左端( 1 , 6 , 11 , 16 )は処理をスキップ */
			if (i%STAGE_WIDTH_NUM == 1) {
				continue;
			}

			/*! 右端( 5 , 10 , 15 , 20 )は処理をスキップ */
			if (i%STAGE_WIDTH_NUM == 0) {
				/*! そのままスキップしても左端になり無駄なループが生じるため iを加算 */
				++i; 
				continue;
			}

			/*! 選択ステージ */
			const bool isSelect = selectNo == i;

			/*! 選択ステージは描画しない */
			const bool isSkip = isSelect && !isTitle;
			if (isSkip) { continue; }

			/*! 座標とテクスチャが入っている可変長配列のインデックス */
			const int index = i - 1;

			/*! 描画 */
			pMiddleObj->Get()->SetModelPos(objPos[index]);
			pMiddleObj->Get()->SetTexture(pTexesTake->GetTex(index));
			pMiddleObj->Render();
		}


		/*! ステージ右端( 5 , 10 , 15 , 20 ) */
		for (int i = STAGE_WIDTH_NUM; i <= NUM; i += STAGE_WIDTH_NUM) {
		

			/*! 選択ステージ */
			const bool isSelect = selectNo == i;

			/*! 選択ステージは描画しない */
			const bool isSkip = isSelect && !isTitle;
			if (isSkip) { continue; }

			/*! 座標とテクスチャが入っている可変長配列のインデックス */
			const int index = i - 1;
			/*! 描画 */
			pSideObj->Get()->SetModelPos(objPos[index]);
			pSideObj->Get()->SetTexture(pTexesTake->GetTex(index));
			pSideObj->Render();
		}

		/************************************/
		/*		  選択ステージの描画		*/
		/************************************/

		/*! 選択しているステージが両端のオブジェクト */
		if (!isTitle) {
			const bool isLeftEnd	= selectNo % STAGE_WIDTH_NUM == 1;	/*!< 左端 */
			const bool isRightEnd	= selectNo % STAGE_WIDTH_NUM == 0;	/*!< 右端 */
			const bool isBothEnd	= isLeftEnd && isRightEnd;			/*!< 両端 */
			const int index			= selectNo - 1;		/*!< 可変長配列のインデックス */

			/*! 選択オブジェクトが 端 と 中央のどちらなのか判断 */
			auto obj = isBothEnd ? pSideObj : pMiddleObj;
			
			/*! 座標補正 */
			XMFLOAT3 pos = objPos[index];
			pos.y += SELECT_STAGE_UP_VALUE;

			/*! 描画 */
			obj->Get()->SetModelPos(pos);
			obj->Get()->SetTexture(pTexesTake->GetTex(index));
			obj->Render();
		}

		/*! ロゴ */
		pSprites["Logo"]->Render();

		/*! UI */
		if (isTitle) {
			pSprites["Title"]->Render();
		}
		else {
			pSprites["Title_None"]->Render();
		}
	}

}

/*!
	@brief	破棄
*/
void StageSelectScene::Destroy()
{
}

/*!
	@brief	入力
*/
void StageSelectScene::Input()
{
	auto& input = InputManager::GetInstance();
	auto& sound = SoundStorage::GetInstance();

	/*****************************/
	/*		↓カーソル操作↓	 */
	/*****************************/

	/*! 左 */
	if (input.GetLeftDown()) {

		/*! タイトル位置にカーソルが存在 */
		if (eState == SelectState::RETURN_TTILE) {

			/*! 右端のステージの番号の算出 */
			const int y = (selectNo - 1) / STAGE_WIDTH_NUM;	

			/*! 現在選択している段の右端の番号 */
			const int index = STAGE_WIDTH_NUM + y * STAGE_WIDTH_NUM;

			selectNo = index;
			eState = SelectState::SELECT_STAGE;
		}
		/*! ステージ選択中 */
		else {

			/*! 左端のステージ */
			if (selectNo%STAGE_WIDTH_NUM == 1) {
				eState = SelectState::RETURN_TTILE;
			}
			/*! それ以外 */
			else {
				--selectNo;
			}
		}
		sound.GetWave("MOVE")->Play();
	}

	/*! 右 */
	if (input.GetRightDown()) {

		/*! タイトル位置にカーソルが存在 */
		if (eState == SelectState::RETURN_TTILE) {

			/*! 左端のステージの番号の算出 */
			const int y = (selectNo - 1) / STAGE_WIDTH_NUM;

			/*! 現在選択している段の左端の番号 */
			const int index = 1 + y * STAGE_WIDTH_NUM;

			selectNo = index;
			eState = SelectState::SELECT_STAGE;
		}
		/*! ステージ選択中 */
		else {

			/*! 右端のステージ */
			if (selectNo%STAGE_WIDTH_NUM == 0) {
				eState = SelectState::RETURN_TTILE;
			}
			/*! それ以外 */
			else {
				++selectNo;
			}
		}
		sound.GetWave("MOVE")->Play();
	}

	/*! 上 */
	if (input.GetUpDown()) {

		/*! 最上段なら処理をスキップ */
		const bool isSkip = (selectNo - 1) / STAGE_WIDTH_NUM == (STAGE_HEIGHT_NUM - 1);
		if (isSkip) { return; }

		fEyeTo = {
			fEyeFrom.x + CAMERA_MOVE_OFFSET.x,
			fEyeFrom.y + CAMERA_MOVE_OFFSET.y,
			fEyeFrom.z + CAMERA_MOVE_OFFSET.z,
		};
		sound.GetWave("MOVE")->Play();
	}

	/*! 下 */
	if (input.GetUnderDown()) {

		/*! 最下段なら処理をスキップ */
		const bool isSkip = (selectNo - 1) / STAGE_WIDTH_NUM == 0;
		if (isSkip) { return; }

		fEyeTo = {
			fEyeFrom.x - CAMERA_MOVE_OFFSET.x,
			fEyeFrom.y - CAMERA_MOVE_OFFSET.y,
			fEyeFrom.z - CAMERA_MOVE_OFFSET.z,
		};
		sound.GetWave("MOVE")->Play();
	}

	/*! 決定 */
	if (input.GetDecideDown()) {

		sound.GetWave("ENTER")->Play();
		eState = static_cast<SelectState>(0b100 | eState);			/*!< ステートを決定 */
		FadeManager::GetInstance().StartFadeOutOnly(FADE_FRAME);	/*!< フェードアウトの開始 */
	}
}

/*!
	@brief	カメラの移動(縦移動のみ)
	@detail	カメラの移動を変更したかったらInput関数とココを編集する
*/
void StageSelectScene::CameraMove()
{
	/*! 符号判定 */
	const int sign = fEyeFrom.y < fEyeTo.y ? 1 : -1;
	
	/*! 移動量 */
	const float movePow = CAMERA_MOVE_OFFSET.y / CAMERA_MOVE_FRAME;

	/*! 移動 */
	fEyeFrom.y += movePow * sign;
	
	/************************/
	/*		 移動終了		*/
	/************************/

	/*! ＋ */
	if (sign > 0) {

		if (fEyeFrom.y > fEyeTo.y) {

			fEyeFrom = fEyeTo;
			/*! 選択ステージを上の段のものにする */
			selectNo += STAGE_WIDTH_NUM;	
		}
	}
	/*! － */
	else {

		if (fEyeFrom.y < fEyeTo.y) {

			fEyeFrom = fEyeTo;
			/*! 選択ステージを下の段のものにする */
			selectNo -= STAGE_WIDTH_NUM;
		}
	}
	
	/*! 座標更新 */
	XMFLOAT3 fLook = fEyeFrom;
	fLook.z = 0;/*!< 補正 */

	CameraManager::GetInstance().CameraSet(fEyeFrom, fLook);

}
