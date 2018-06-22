/*!
	@file	StageManager.h
	@date	2018/04/05
	@author	番場 宥輝
	@brief	ステージマネージャー
*/
#pragma once
#include "../../Math/Index.h"
#include "../Block/FixedBlock.h"
#include "../Block/NormalBlock.h"
#include "../Block/LossBlock.h"
#include "../Block/ResonanceBlock.h"
#include "../../Fbx/TakeModelData.h"
#include "../../Fbx/Texes/TexesTake.h"
#include <map>

/*!
	@def	前方宣言
*/
class SquaresManager;

/*!
	@brief	ブロックを管理しステージを構成するクラス
*/
class StageManager
{
public:
	StageManager() {}
	~StageManager() {}

	void Initialize();
	void Finalize();
	void Render();
	void Update();

	/*! セッター */
	void SetLoadData();
	void SetRefData(SquaresManager* ref);

	/*! ゲッター */
	std::vector<NormalBlock*>GetNormalBlocks()const { return nBlocks; }
	std::vector<ResonanceBlock*>GetResonanceBlocks()const { return rBlocks; }
	std::vector<LossBlock*>GetLossBlocks()const { return lBlocks; }
	INDEX GetGoalIndex()const { return goalParam; }

private:
	/*! 参照 */
	SquaresManager * pRef;

	/*! 定数 */
	static const DirectX::XMFLOAT3 GOAL_LEFT_TOP_POS;	/*!< ゴールの左上座標 */
	static const DirectX::XMFLOAT2 GOAL_OFFSET;			/*!< ゴールインデックスが1変動することで変化するオフセット */

	/*! パラメーター */
	INDEX goalParam;/*!< ゴールのインデックス */

	/*! オブジェクト */
	std::vector<FixedBlock*>	fBlocks;/*!< 固定 */
	std::vector<NormalBlock*>	nBlocks;/*!< 通常 */
	std::vector<ResonanceBlock*>rBlocks;/*!< 共鳴 */
	std::vector<LossBlock*>		lBlocks;/*!< 消失 */

	/*! 張替え用テクスチャ */
	TexesTake* pTexesTake;

	/*! レンダー(モデル) */
	std::shared_ptr<TakeModelData> fBlockModel;	/*!< 固定 */
	std::shared_ptr<TakeModelData> nBlockModel;	/*!< 通常 */
	std::shared_ptr<TakeModelData> lBlockModel;	/*!< 共鳴 */
	std::shared_ptr<TakeModelData> rBlockModel;	/*!< 消失 */
	std::shared_ptr<TakeModelData> goalModel;	/*!< ゴール */

	/*! 可変長配列への追加用セッター */
	void AddFBlock(int id,INDEX index);
	void AddNBlock(int id,INDEX index);
	void AddLBlock(int id,INDEX index);
	void AddRBlock(int id,INDEX index);
};