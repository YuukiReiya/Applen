#include "LossBlock.h"

/*!
	@brief	���O���
*/
using namespace std;
using namespace DirectX;

/*!
	@def	�萔�錾
*/
const float LossBlock::MAX_ALPHA = 0.6f;		/*!< �`�悷��I�u�W�F�N�g�̃��l�̍ő�l */
const float LossBlock::MIN_ALPHA = 0.4f;		/*!< �`�悷��I�u�W�F�N�g�̃��l�̍ŏ��l */
const int LossBlock::TRANSRATION_FRAME = 150;	/*!< ���l���J�ڂ���t���[�� */

/*! �u�����h���銄��(���v10�ɂȂ�悤�ݒ肷��) */
const XMINT2 LossBlock::BLEND_RATIO{
	8,	/*!< �\�����Ԃ̊��� */
	2,	/*!< ��\�����Ԃ̊��� */
};

/*!
	@brief	�R���X�g���N�^
*/
LossBlock::LossBlock():BlockBase(),alpha(MAX_ALPHA)
{
	sign = -1;/*!< �����̓��l�̌��Z������� */
	count = 0;
}

/*!
	@brief	�f�X�g���N�^
*/
LossBlock::~LossBlock()
{
}

/*!
	@brief	������
*/
void LossBlock::Initialize()
{
	BlockBase::Initialize();
	/*! �����񐔂̐ݒ� */
	lossNum = id / ID_OPTION1;
	/*! �j���t���O */
	isDelete = false;
	/*! ���� */
	sign = -1;/*!< �ŏ��̓��l�̌��Z���珈������ */
}

/*!
	@brief	�X�V
*/
void LossBlock::Update()
{
	Move();
	Blend();
}

/*!
	@brief	��ړ��J�n����
	@detail	�R�[���o�b�N�֐��ɏ����񐔂̌���������ǉ����������s�킹��
*/
void LossBlock::StartMoveUp(std::function<void()> func)
{
	function<void()> compileFunc;

	/*! �֐��̃��b�s���O */
	compileFunc = [=] 
	{
		if (func != nullptr) {
			func();
		}
		--lossNum;
		if (lossNum <= 0) {
			isDelete = true;
		}
	};

	BlockBase::StartMoveUp(compileFunc);
}

/*!
	@brief	���ړ��J�n����
	@detail	�R�[���o�b�N�֐��ɏ����񐔂̌���������ǉ����������s�킹��
*/
void LossBlock::StartMoveDown(std::function<void()> func)
{
	function<void()> compileFunc;

	/*! �֐��̃��b�s���O */
	compileFunc = [=]
	{
		if (func != nullptr) {
			func();
		}
		--lossNum;
		if (lossNum <= 0) {
			isDelete = true;
		}
	};


	BlockBase::StartMoveDown(compileFunc);
}

/*!
	@brief	���l�̃u�����h
*/
void LossBlock::Blend()
{
	/*! �J�E���^�̉��Z */
	++count;

	/*! ���� */
	int A = TRANSRATION_FRAME * BLEND_RATIO.x / 10;	/*!< ���Z���̑J�ڃt���[���̕\�����Ԋ��� */
	int B = TRANSRATION_FRAME * BLEND_RATIO.y / 10;	/*!< ���Z���̑J�ڃt���[���̔�\�����Ԋ��� */
	int C = A + B + B;								/*!< ���Z���̑J�ڃt���[���̕\�����Ԋ��� */
	int D = A + C;									/*!< ���Z���̑J�ڃt���[���̔�\�����Ԋ��� */

	float val;/*!< ���l�̉��Z�ʂ�\���ϐ� */

	const float difference = MAX_ALPHA - MIN_ALPHA;/*!< MAX��MIN�̍��� */

	/*!
		���l�̍�����Ԃ𒷂��A�Ⴂ��Ԃ�Z���������̂Ńt���[���ɉ�����
		���l�̕ψق��ɂ₩�ȏ��/��������Ԃ�2��ŕψق��s�킹��
	*/

	/*! �ɂ₩�Ȍ��Z */
	if (count < A) {
		val = difference / A;
		sign = -1;
	}
	/*! ���������Z */
	else if (count < A + B) {
		val = difference / B;
		sign = -1;
	}
	/*! ���������Z */
	else if (count < C) {
		val = difference / B;
		sign = 1;
	}
	/*! �ɂ₩�ȉ��Z */
	else if (count < D) {
		val = difference / A;
		sign = 1;
	}
	/*! 1�t���[���ҋ@���덷������ */
	else {
		val = 0;
	}

	/*! ���l�̕ψ� */
	alpha += val * sign;

	/*! �J�E���^�̃��Z�b�g */
	if (count == D) {
		count = 0;
	}
}
