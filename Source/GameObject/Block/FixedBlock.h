/*!
	@file	StageEmpty.h
	@date	2018/04/04
	@author	番場 宥輝
	@brief	ステージ用のブロック
*/
#pragma once
#include "../../math/Index.h"
#include "../../Common.h"
#include "BlockBase.h"

/*!
	@brief	固定ブロック
*/
class FixedBlock
	:public BlockBase
{
public:
	FixedBlock() {}
	FixedBlock(INDEX val) :BlockBase(val) {}
	~FixedBlock()override;

	void Update() {};
};