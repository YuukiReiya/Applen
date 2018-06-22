#include "BlockBase.h"

/*! 
	@brief	���O���
*/
using namespace DirectX;

/*! 
	@def	�萔�錾
*/

/*! ������W */
const XMFLOAT3 BlockBase::LEFT_TOP_POSITION = {
	-5.25f,	/*!< x */
	3.35f,	/*!< y */
	1		/*!< z */
};

/*! �I�u�W�F�N�g�Ԃ̃I�t�Z�b�g */
const XMFLOAT2 BlockBase::OFFSET = {
	1.82f,	/*!< x */
	1.82f,	/*!< y */
};

/*! 1�t���[��������̈ړ���(���������^��) */
const float BlockBase::MOVE_RER_FRAME = 0.2f;

/*! ID�̌��w��I�t�Z�b�g */
const int BlockBase::ID_OPTION1 = 100000;	/*!< ������1�����ڂ̌����w�肷��I�t�Z�b�g */
const int BlockBase::ID_OPTION2 = 10000;	/*!< ������2�����ڂ̌����w�肷��I�t�Z�b�g */
const int BlockBase::ID_OPTION3 = 1000;		/*!< ������3�����ڂ̌����w�肷��I�t�Z�b�g */
const int BlockBase::ID_OPTION4 = 100;		/*!< ������4�����ڂ̌����w�肷��I�t�Z�b�g */
const int BlockBase::ID_OPTION5 = 10;		/*!< ������5�����ڂ̌����w�肷��I�t�Z�b�g */
const int BlockBase::ID_OPTION6 = 1;		/*!< ������6�����ڂ̌����w�肷��I�t�Z�b�g */

/*!
	@brief	������
*/
void BlockBase::Initialize()
{
	from.x = LEFT_TOP_POSITION.x + index.x*OFFSET.x;
	from.y = LEFT_TOP_POSITION.y - index.y*OFFSET.y;
	from.z = LEFT_TOP_POSITION.z;
	to = from;
	eState = MoveState::WAIT;
	actionFinishedFunc = nullptr;
}

/*!
	@brief	�j��
*/
void BlockBase::Finalize()
{
}

/*!
	@brief	��ړ��J�n����
*/
void BlockBase::StartMoveUp(std::function<void()> func)
{
	actionFinishedFunc = func;
	to = from;
	to.y += OFFSET.y;
	eState = MoveState::UP;
	index.y -= 1;
}

/*!
	@brief	���ړ��J�n����
*/
void BlockBase::StartMoveDown(std::function<void()> func)
{
	actionFinishedFunc = func;
	to = from;
	to.y -= OFFSET.y;
	eState = MoveState::DOWN;
	index.y += 1;
}

/*!
	@brief	�ړ�
	@detail	���L�ړ��֐����b�s���O
*/
void BlockBase::Move()
{
	/*! �X�e�[�g�؂�ւ� */
	switch (eState)
	{
		/*! �ړ��ҋ@ */
	case BlockBase::WAIT:
		break;
		/*! ��ړ� */
	case BlockBase::UP:
		MoveUp();
		break;
		/*! ���ړ� */
	case BlockBase::DOWN:
		MoveDown();
		break;
	default:
		assert("Invalid value");
		break;
	}

}

/*!
	@brief	��ړ�
*/
void BlockBase::MoveUp()
{	
	/*! �ړ� */
	if (from.y < to.y) {
		from.y += MOVE_RER_FRAME;
	}
	/*! �ړ��I�� */
	else {
		from = to;
		eState = MoveState::WAIT;
		/*! �R�[���o�b�N�֐��̌Ăяo�� */
		if (actionFinishedFunc != nullptr) {
			actionFinishedFunc();			/*!< ���s */
			actionFinishedFunc = nullptr;	/*!< ������ */
		}
	}
}

/*!
	@brief	���ړ�
*/
void BlockBase::MoveDown()
{
	/*! �ړ� */
	if (from.y > to.y) {
		from.y -= MOVE_RER_FRAME;
	}
	/*! �ړ��I�� */
	else {
		from = to;
		eState = MoveState::WAIT;
		/*! �R�[���o�b�N�֐��̌Ăяo�� */
		if (actionFinishedFunc != nullptr) {
			actionFinishedFunc();			/*!< ���s */
			actionFinishedFunc = nullptr;	/*!< ������ */
		}
	}
}
