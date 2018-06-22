/*!
	@file	ResonanceBlock.h
	@date	2018/05/09
	@author	�ԏ� �G�P
	@brief	���A�N�V�������s���I�u�W�F�N�g
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

	/*! �萔 */
	static const float PI;/*!< �� */
	static const DirectX::XMFLOAT3 ROTATION_PER_FRAME;	/*!< 1�t���[��������̉�]�� */

	DirectX::XMFLOAT3 rot;

};