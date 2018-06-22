/**
	* @file		TitleScene.h
	* @brief	タイトルシーン
	* @author	番場 宥輝
	* @date　	2018/02/20
*/
#pragma once
#include "../Manager/SceneManager.h"
#include "../../Camera/CameraManager.h"
#include "../../BackGround/TitleBackGround.h"
#include "../../Sprite/SpriteBase.h"
#include "../../Sprite/Sprite.h"


class TitleScene final
	: public Scene
{
public:
	TitleScene();
	~TitleScene() {};

	void Init();
	SceneBase* Update(SceneManager* root);
	void Render();
	void Destroy();
private:

	/*! 定数 */
	static const int FADE_FRAME;

	/*! タイトルメニュー */
	enum TitleMenu :int8_t {
		/*!
		下3桁をメニューの選択に4桁目は決定に判定
		ex)
		00000100:"EXIT"選択状態
		↓
		00001100:"EXIT"決定状態
		*/
		TITLE_PLAY = 0b0001,
		TITLE_CREDIT = 0b0010,
		TITLE_EXIT = 0b0100,
	}eMenu;

	/*! 下記ステートの基底 */
	struct StateBase {
		StateBase() {};
		StateBase(TitleScene* scene) :next(nullptr) { this->scene = scene; }
		virtual void Initialize() {};
		virtual void Finalize()		= 0;
		virtual void Update()		= 0;
		virtual void Render()		= 0;
		SceneBase* next;
	protected:
		TitleScene * scene;
	};

	/*! "PLAY"選択ステート */
	struct PlayState
		:public StateBase
	{
		PlayState(TitleScene* scene) :StateBase(scene) {}
		void Initialize();
		void Finalize();
		void Update();
		void Render();
	};

	/*! "CREDIT"選択ステート */
	struct CreditState
		:public StateBase
	{
		CreditState(TitleScene* scene) :StateBase(scene) {}
		void Initialize();
		void Finalize();
		void Update();
		void Render();
	private:
		/*! 定数 */
		static const std::string HASH_NAME;				/*!< 表示するテクスチャのハッシュ名 */
		static const int SCALE_FRAME;					/*!< 拡大縮小にかけるフレーム */
		static const DirectX::XMFLOAT2 POSITION_OFFSET;	/*!< スケール変化に伴い変化する座標のオフセット */
		static const DirectX::XMFLOAT2 TEXTURE_SCALE;	/*!< テクスチャのスケール */
		
		/*! 変数 */
		int count;
		bool isOpen;

		/*! 関数 */
		void Scaling();
		void Close();
	};

	/*! "EXIT"選択ステート */
	struct ExitState
		:public StateBase
	{
		ExitState(TitleScene* scene) :StateBase(scene) {}
		void Initialize();
		void Finalize();
		void Update();
		void Render();
	};


	/*! 隠蔽関数 */
	void Input();

	/*! メンバ変数 */
	StateBase* pState;			/*!< ステート用変数 */
	umap_str_sprite pSprites;	/*!< 表示テクスチャ */
	TitleBackGround* pTbg;

};