/*!
	@file	StageEmpty.h
	@date	2018/04/04
	@author	�ԏ� �G�P
	@brief	�X�e�[�W�p�̃u���b�N
*/
#pragma once
#include "../../math/Index.h"
#include "../../Common.h"
#include "BlockBase.h"

/*!
	@brief	�Œ�u���b�N
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