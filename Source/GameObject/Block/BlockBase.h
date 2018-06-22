/*!
	@file	BlockBase.h
	@date	2018/04/09
	@author	�ԏ� �G�P
	@brief	�u���b�N�̊��N���X
*/
#pragma once
#include "../../Math/Index.h"
#include <Windows.h>
#include <DirectXMath.h>
#include <functional>

/*! 
	@brief	�u���b�N�I�u�W�F�N�g�̊��N���X
*/

class BlockBase 
{
public:
	/*! �R���X�g���N�^���f�X�g���N�^ */
	BlockBase() {}
	BlockBase(INDEX index) { this->index = index; }
	virtual ~BlockBase() {}

	/*! �ړ���� */
	enum MoveState :int32_t {
		WAIT	= 0,	/*!< �ҋ@ */
		UP,				/*!< ��ړ��� */
		DOWN			/*!< ���ړ��� */
	};

	/*! ���J�֐� */
	virtual void Initialize();
	virtual void Finalize();
	virtual void StartMoveUp(std::function<void()> func = nullptr);
	virtual void StartMoveDown(std::function<void()> func = nullptr);

	/*! �������z�֐� */
	virtual void Update() = 0;

	/*! �萔 */
	/*! ��ID��7����(�ō�)�̓A���t�@�x�b�g�̂��ߏ����Ă��� */
	static const int ID_OPTION1;	/*!< ��(ID�̍�����1�����ڂ̌����w�肷��I�t�Z�b�g) */
	static const int ID_OPTION2;	/*!< ��(ID�̍�����2�����ڂ̌����w�肷��I�t�Z�b�g) */
	static const int ID_OPTION3;	/*!< ��(ID�̍�����3�����ڂ̌����w�肷��I�t�Z�b�g) */
	static const int ID_OPTION4;	/*!< ��(ID�̍�����4�����ڂ̌����w�肷��I�t�Z�b�g) */
	static const int ID_OPTION5;	/*!< ��(ID�̍�����5�����ڂ̌����w�肷��I�t�Z�b�g) */
	static const int ID_OPTION6;	/*!< ��(ID�̍�����6�����ڂ̌����w�肷��I�t�Z�b�g) */

	/*! �Z�b�^�[ */
	void SetID(int id)			{ this->id = id; }
	void SetIndex(INDEX index)	{ this->index = index; }

	/*! �Q�b�^�[ */
	DirectX::XMFLOAT3 GetPos()const { return from; }
	INDEX GetIndex()const		{ return index; }
	int GetID()const			{ return id; }
protected:
	/*! ��ی�֐� */
	void Move();		/*!< �ړ��֐� */
	void MoveUp();		/*!< ��ړ� */
	void MoveDown();	/*!< ���ړ� */

	/*! �萔 */
	static const DirectX::XMFLOAT3 LEFT_TOP_POSITION;	/*!< ������W */
	static const DirectX::XMFLOAT2 OFFSET;				/*!< �I�u�W�F�N�g�Ԃ̕� */
	static const float MOVE_RER_FRAME;					/*!< 1�t���[��������̈ړ��� */

	INDEX					index;					/*!< �C���f�b�N�X */
	int						id;						/*!< ID */
	MoveState				eState;					/*!< �ړ���� */
	DirectX::XMFLOAT3		from;					/*!< �ړ������W(�����W) */
	DirectX::XMFLOAT3		to;						/*!< �ړ�����W */
	std::function<void()>	actionFinishedFunc;		/*!< �A�N�V�����I�����Ă΂��R�[���o�b�N�֐� */
};