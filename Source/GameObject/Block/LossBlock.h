/*!
	@file	LossBlock.h
	@date	2018/05/20
	@author	�ԏ� �G�P
	@brief	�����A�N�V�������s���I�u�W�F�N�g
*/
#pragma once
#include "BlockBase.h"

/*!
	@brief	�K��񐔓��������������u���b�N
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

	/*! �萔 */
	static const float MAX_ALPHA;
	static const float MIN_ALPHA;
	static const int TRANSRATION_FRAME;/*!< ���l�̂̑J�ڃt���[�� */
	static const DirectX::XMINT2 BLEND_RATIO;/*!< �u�����h���銄�� */

	/*! �p�����[�^ */
	int lossNum;	/*!< ������ */
	bool isDelete;	/*!< �폜�t���O */
	float alpha;	/*!< ���l */
	int sign;		/*!< ���� */
	int count;
};