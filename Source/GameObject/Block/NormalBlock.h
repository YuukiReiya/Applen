/*!
	@file	NormalBlock.h
	@date	2018/04/09
	@author	�ԏ� �G�P
	@brief	�ʏ�A�N�V�������s���I�u�W�F�N�g
*/
#pragma once
#include "BlockBase.h"

/*!
	@brief	�ʏ�ړ����s���u���b�N
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