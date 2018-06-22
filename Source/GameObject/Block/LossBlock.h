/*!
	@file	LossBlock.h
	@date	2018/05/20
	@author	番場 宥輝
	@brief	消失アクションを行うオブジェクト
*/
#pragma once
#include "BlockBase.h"

/*!
	@brief	規定回数動いたら消失するブロック
*/
class LossBlock
	:public BlockBase
{
public:
	LossBlock();
	LossBlock(INDEX val) :BlockBase(val) {}
	~LossBlock()override;

	void Initialize()override;
	void Update()override;

	void StartMoveUp(std::function<void()> func = nullptr)override;
	void StartMoveDown(std::function<void()> func = nullptr);

	bool GetIsDelete()const { return isDelete; }
	float GetAlpha()const { return alpha; }
private:
	void Blend();

	/*! 定数 */
	static const float MAX_ALPHA;
	static const float MIN_ALPHA;
	static const int TRANSRATION_FRAME;/*!< α値のの遷移フレーム */
	static const DirectX::XMINT2 BLEND_RATIO;/*!< ブレンドする割合 */

	/*! パラメータ */
	int lossNum;	/*!< 消失回数 */
	bool isDelete;	/*!< 削除フラグ */
	float alpha;	/*!< α値 */
	int sign;		/*!< 符号 */
	int count;
};