/*!
	@file	NormalBlock.h
	@date	2018/04/09
	@author	番場 宥輝
	@brief	通常アクションを行うオブジェクト
*/
#pragma once
#include "BlockBase.h"

/*!
	@brief	通常移動を行うブロック
*/
class NormalBlock
	:public BlockBase 
{
public:
	NormalBlock() :BlockBase() {}
	NormalBlock(INDEX val) :BlockBase(val) {}
	~NormalBlock()override;

	void Update()override;
};