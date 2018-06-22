/*!
	@file	StageSelectScene.h
	@date	2018/05/14
	@author	番場 宥輝
	@brief	ステージセレクトシーン
*/
#pragma once
#include "../Scene.h"
#include "../../Fbx/TakeModelData.h"
#include "../../Sprite/Sprite.h"
#include "../../Fbx/Texes/TexesTake.h"


class StageSelectScene :
	public Scene
{
public:
	StageSelectScene();
	~StageSelectScene();

	void Init()override;
	SceneBase* Update(SceneManager* root)override;
	void Render()override;
	void Destroy()override;

	/*! 選択ステージを変更する */
	static inline void SetSelectStage(int stage) {
		selectNo = stage;
	}
private:
	///*! 列挙体 */
	//enum InputState {
	//	STAGE_SELECTING,
	//	TITLE_SELECTING,
	//	STAGE_DECIDE,
	//	TITLE_DECIDE,
	//	UP_WAIT,
	//	DOWN_WAIT,
	//}eInputState;

	//void Input();

	//static const int STAGE_ROW_X;
	//static const int STAGE_ROW_Y;
	//static const float STAGE_SELECT_UP_VALUE;
	//static const float CAMERA_TRANSFORMATION_HEIGHT;

	//static const float BGM_WAIT_START_SECOND;
	//int bgmCount;
	//DirectX::XMFLOAT3 toCameraPos;
	//std::vector<DirectX::XMFLOAT3> ObjPos;

	//std::shared_ptr<TakeModelData> SideObj;		/*!< 両端(１と５)のステージセレクトオブジェクト */
	//std::shared_ptr<TakeModelData> MiddleObj;	/*!< ２～４までのステージセレクトオブジェクト */
	//uint8_t stageNo;
	//umap_str_sprite pSprites;
	//TexesTake* pTexesTake;
	//int moveFrame;
	//float movePower;

	////////////////////////////////////////

	/*! 選択状態 */
	enum SelectState :int8_t {
		/*!
		下3桁をメニューの選択に4桁目は決定に判定
		ex)
		010:"TITLE"
		↓
		110:"TITLE"決定状態
		*/
		SELECT_STAGE = 0b01,	/*!< ステージ */
		RETURN_TTILE = 0b10,	/*!< タイトル */
	}eState;

	/*! 定数 */
	static const int FADE_FRAME;				/*!< フェードさせるフレーム */
	static const int STAGE_WIDTH_NUM;			/*!< 横1段に並ぶステージの数 */
	static const int STAGE_HEIGHT_NUM;			/*!< 縦に並ぶステージの数 */
	static const int CAMERA_MOVE_FRAME;			/*!< カメラを移動させるフレーム */
	static const float SELECT_STAGE_UP_VALUE;	/*!< 選択ステージの上昇量 */

	static const DirectX::XMFLOAT3 CAMERA_MOVE_OFFSET;	/*!< カメラの移動オフセット */

	void Input();
	void CameraMove();

	/*! 静的メンバ変数 */
	static int selectNo;		/*!< 選択しているステージの番号 */

	/*! メンバ変数 */
	std::shared_ptr<TakeModelData> pSideObj;		/*!< 両端のブロック */
	std::shared_ptr<TakeModelData> pMiddleObj;	/*!< 両端を除いた中央のブロック */
	TexesTake* pTexesTake;						/*!< ブロックの変更用UV */
	umap_str_sprite pSprites;					/*!< スプライト */
	std::vector<DirectX::XMFLOAT3>objPos;		/*!< ステージオブジェクトの表示座標 */

	DirectX::XMFLOAT3 fEyeFrom;					/*!< 視点位置 */
	DirectX::XMFLOAT3 fEyeTo;					/*!<  移動先座標の視点位置 */
};

