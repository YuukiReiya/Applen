/*!
	@file	MainScene.h
	@date	2018/04/04
	@author	番場 宥輝
	@brief	ゲームのメインシーン
*/
#pragma once
#include "../Scene.h"
#include "../../Camera/CameraManager.h"
#include "../../GameObject/Player/Player.h"
#include "../../GameObject/StageManager/StageManager.h"
#include "../../GameObject/Squares/SquaresManager.h"
#include "../../Sprite/SpriteBase.h"
#include "../../Sprite/Sprite.h"
#include "../../BackGround/BackGround.h"


class MainScene final:
	public Scene
{
public:
	MainScene();
	~MainScene();


	void Init()override;
	SceneBase* Update(SceneManager* root)override;
	void Render()override;
	void Destroy()override;
private:

	/*! 下記ステートの基底 */
	struct StateBase
	{
		StateBase(MainScene*main) :next(nullptr) { this->main = main; }
		virtual void Initialize()	= 0;
		virtual void Finalize()		= 0;
		virtual StateBase* Update()	= 0;
		virtual void Render()		= 0;
		SceneBase* next;
	protected:
		MainScene* main;
	};

	/*! ゲーム中のステート */
	struct GameState :
		public StateBase
	{
		GameState(MainScene*main) :StateBase(main) {}
		void Initialize()override;
		void Finalize()override;
		StateBase* Update()override;
		void Render()override;
	};

	/*! ポーズ中のステート */
	struct PauseState :
		public StateBase
	{
		PauseState(MainScene* main) :StateBase(main) {}
		void Initialize()override;
		void Finalize()override;
		StateBase* Update()override;
		void Render()override;
		/*! ポーズメニュー */
		enum PauseMenu:int8_t {
			/*!
				下4桁をメニューの選択に5桁目は決定に判定
				ex)
					00001000:タイトル選択状態
							↓
					00011000:タイトル決定状態
			*/
			PAUSE_PLAY			= 0b0001,	/*!< ゲーム再開 */
			PAUSE_RETRY			= 0b0010,	/*!< ステージをリトライ */
			PAUSE_STAGESELECT	= 0b0100,	/*!< ステージセレクトへ戻る */
			PAUSE_TITLE			= 0b1000,	/*!< タイトルへ戻る */
		}eMenu;

	private:
		void Input();
	};

	/*! クリア時のステート */
	struct ClearState:
		public StateBase
	{
		ClearState(MainScene* main) :StateBase(main) {}
		void Initialize()override;
		void Finalize()override;
		StateBase* Update()override;
		void Render()override;
		/*! クリアメニュー */
		enum ClearMenu :int8_t {
			/*!
			下3桁をメニューの選択に4桁目は決定に判定
			ex)
			00000100:タイトル選択状態
			↓
			00001100:タイトル決定状態
			*/
			CLEAR_NEXTSTAGE		= 0b0001,	/*!< 次のステージ */
			CLEAR_STAGESELECT	= 0b0010,	/*!< ステージセレクト */
			CLEAR_TITLE			= 0b0100,	/*!< タイトル */
		}eMenu;
	private:
		static const int WAIT_FRAME;
		int count;
		void Input();
		bool isLastStage;
	};

	/*! 定数 */
	static const int FADE_FRAME;

	/*! ステート */
	StateBase* pState;

	/*! マネージャークラス */
	SquaresManager*		pSquareManager;
	StageManager*	pStageManager;
	Player*			pPlayer;

	/*! スプライト */
	umap_str_sprite pSprites;

	/*! 背景 */
	BackGround* pBack;

	/*! 隠蔽関数 */
	void StartBGM();
	void StopBGM();
};

