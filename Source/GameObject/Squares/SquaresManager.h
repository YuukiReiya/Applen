/*!
	@file	GridManager.h
	@date	2018/04/05
	@author	番場 宥輝
	@brief	グリッドマネージャー
*/
#pragma once
#include <vector>
#include "../../Math/Index.h"
#include "../Player/Player.h"
#include "../StageManager/StageManager.h"


/*!
	@brief	ゲーム上をマス管理するクラス
*/

/*! デバッグ描画用マクロ */
//#define DebugDraw

class SquaresManager 
{
public:
	void Initialize();
	void Finalize();
	void Update();
#ifdef DebugDraw
	void Render();
#endif
	void UpAction(INDEX index);
	void DownAction(INDEX index);

	/*! セッター */
	void SetRef(Player* player, StageManager* stageManager);
	void SetGridStateNone(INDEX index);
	/*! ゲッター */
	bool GetIsBlockMoving()const;			/*!< ブロックが移動中か判定 */
	bool GetIsBlock(INDEX index)const;		/*!< インデックス上にブロックが存在するか判定 */
	bool GetIsMoveBlock(INDEX index)const;	/*!< インデックス上に動かせるブロックが存在するか判定 */
	bool GetIsClear()const;					/*!< ステージクリアしたかどうか(ゴールのインデックスにプレイヤーのインデックスがあるか) */
	bool GetIsLastStage()const;				/*!< 最終ステージかどうか判定 */
private:
	template<typename UpBlocks>
	void BlockUp(UpBlocks blocks, INDEX index, std::function<void()>func);
	template<typename DownBlocks>
	void BlockDown(DownBlocks blocks, INDEX index, std::function<void()>func);

	/*! 列挙体 */
	enum GridState :int8_t {
		NONE,		/*!< ブロック無し */
		FIXED,		/*!< 固定 */
		NORMAL,		/*!< 通常 */
		LOSS,		/*!< 消失 */
		RESONANCE,	/*!< 共鳴 */
	};

	/*! 別名宣言 */
	typedef std::function<void()> FPVOID;

	/*! パラメータ */
	bool isMovingBlock;

	/*! 参照 */
	Player*			pPlayerRef;			/*!< プレイヤーのポインタ */
	StageManager*	pStageRef;			/*!< ステージのポインタ */

	/*! 可変長配列 */
	std::vector<std::vector<GridState>>	eGridState;	/*!< グリッドの状態を格納 */
};