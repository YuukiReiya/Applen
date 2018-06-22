/*!
	@file	ResonanceBlock.h
	@date	2018/05/09
	@author	番場 宥輝
	@brief	共鳴アクションを行うオブジェクト
*/
#pragma once
#include "BlockBase.h"

class ResonanceBlock
	:public BlockBase
{
public:
	ResonanceBlock();
	ResonanceBlock(INDEX val) :BlockBase(val) {}
	~ResonanceBlock()override;

	void Update()override;

	DirectX::XMFLOAT3 GetRot()const { return rot; }
	void SetRot(DirectX::XMFLOAT3 rot) { this->rot = rot; }
private:
	void Rotation();

	/*! 定数 */
	static const float PI;/*!< π */
	static const DirectX::XMFLOAT3 ROTATION_PER_FRAME;	/*!< 1フレームあたりの回転量 */

	DirectX::XMFLOAT3 rot;

};