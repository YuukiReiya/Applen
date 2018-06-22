#include <string>
#include <algorithm>
#include "Player.h"
#include "../../StageLoader/StageLoader.h"
#include "../Squares/SquaresManager.h"
#include "../../Input/InputManager/InputManager.h"

/*!
	@brief	�}�N������
	@detail	�A���S���Y���w�b�_��max/min���g������
*/
#undef max
#undef min

/*!
	@brief	���O���
*/
using namespace std;
using namespace DirectX;

/*!
	@def	�萔�錾
*/

/*! ������W */
const XMFLOAT3 Player::LEFT_TOP_OFFSET= {
	-2.4f,	/*!< x */
	1.6f,	/*!< y */
	0.5f	/*!< z */
};

/*! ���f���̉�]�l */
const XMFLOAT3 Player::MODEL_ROTATION = { 
	-1.6f,	/*!< yaw */
	0.0f,	/*!< pith */
	0.0f	/*!< roll */
};

/*! 1�}�X������̕� */
//XMFLOAT2 Player::OFFSET= { 
//	0.85f,
//	0.85f 
//};
const float Player::MOVE_MAX_SPEED = 0.1f;		/*!< �ړ����x��� */
const float Player::MOVE_PER_FRAME = 0.05f;		/*!< 1�t���[���ӂ�̃x�[�X�ړ��� */
const float Player::SPEED_ADD_VALUE = 0.02f;	/*!< �ړ����ɖ��t���[�� (1+(this))^2 �̒l��MOVE_PER_FRAME�ɏ�Z����� */
const int Player::WAIT_FRAME = 90;				/*!< �A�j���[�V�����̑ҋ@�t���[��(���̃t���[���ҋ@���Ă���A�N�V��������) */

/*!
	@brief	�R���X�g���N�^
*/
Player::Player()
{
	pModel = nullptr;
	pRefInfo = nullptr;
	delayFunc = nullptr;

	OFFSET = {
		0.85f,
		0.85f
	};
}

/*!
	@brief	�f�X�g���N�^
*/
Player::~Player()
{
}

/*!
	@brief	������
*/
void Player::Initialize()
{
	/*! �C���X�^���X���� */
	{
		pModel = new PlayerModel();
	}

	/*! ������ */
	{
		/*! ���f�� */
		pModel->Init();
		pModel->SetMotionNum(2);

		/*! �p�����[�^ */
		speed = 0;
		additionValueBase = 0;	/*!< ��b���Z�l */
		eMoveState = MoveState::WAIT;
		isInput = false;

		if (StageLoader::GetInstance().GetLoadedStageNo() >= 8)
		{
			OFFSET = { 0.87f,0.87f };

			if (StageLoader::GetInstance().GetLoadedStageNo() >= 12)
			{
				OFFSET = { 0.88f,0.88f };
			}
		}
	}

}

/*!
	@brief	���
*/
void Player::Finalize()
{
}

/*!
	@brief	�X�V
*/
#include "../../Input/Keyboard/Keyboard.h"
void Player::Update()
{
	/*! �X�V���� */
	Move();
	Input();
	Wait();

	/*! ��]�X�V */
	rot = MODEL_ROTATION;
	rot.x *= dir;
	pModel->SetRot(rot.x,rot.y,rot.z);

	/*! ���W�n�X�V */
	pModel->SetPos(pos.x,pos.y,pos.z);
}

/*!
	@brief	�`��
*/
void Player::Render()
{
	pModel->Render();
}

/*!
	@brief	���[�h�f�[�^�̃Z�b�g
*/
void Player::SetLoadData()
{
	/*! �ǂݍ��񂾃f�[�^��ݒ� */
	auto& load = StageLoader::GetInstance();
	index = load.GetPlayerInfo().index;
	dir = load.GetPlayerInfo().dir;

	/*! ���W�ϊ� */
	Convert(&pos, index);
	/*! ��] */
	rot = MODEL_ROTATION;
	rot.x *= dir;
	pModel->SetRot(rot.x, rot.y, rot.z);
	/*! ���W */
	pModel->SetPos(pos.x, pos.y, pos.z);
}

/*!
	@brief	�ړ�
*/
void Player::Move()
{
	switch (eMoveState)
	{
	case Player::WAIT:
		break;
	case Player::LEFT:
		pModel->SetMotionNum(1);
		MoveLeft();
		break;
	case Player::RIGHT:
		pModel->SetMotionNum(1);
		MoveRight();
		break;
	case Player::UP:
		MoveUp();
		break;
	case Player::DOWN:
		MoveDown();
		break;
	}
}

/*!
	@brief	���ړ�
*/
void Player::MoveLeft()
{
	/*! �ړ��� */
	if (pos.x > nextPos.x) {
		
		/*! ���t���[���ړ����Z�ʂ𑝂₵�Ă��� */
		additionValueBase += SPEED_ADD_VALUE;
		
		/*! ���x��1�t���[���ӂ�̈ړ��ʁ~�ړ����Z�ʗʂ��Q�悵���l */
		speed = MOVE_PER_FRAME * powf(1 + additionValueBase, 2);

		/*! ���x����ɓ��B�����瑬�x���ۂ߂� */
		speed = min(speed, MOVE_MAX_SPEED);

		/*! �ړ� */
		pos.x -= speed;
	}
	/*! �ړ��I�� */
	else {
		index.x -= 1;					/*!< �C���f�b�N�X�X�V */
		eMoveState = MoveState::WAIT;	/*!< �ړ���Ԃ�ҋ@�� */
		additionValueBase = 0;			/*!< ���Z�ʂ̏����� */
	}
}

/*!
	@brief	�E�ړ�
*/
void Player::MoveRight()
{	
	/*! �ړ��� */
	if (pos.x < nextPos.x) {

		/*! ���t���[���ړ����Z�ʂ𑝂₵�Ă��� */
		additionValueBase += SPEED_ADD_VALUE;

		/*! ���x��1�t���[���ӂ�̈ړ��ʁ~�ړ����Z�ʗʂ��Q�悵���l */
		speed = MOVE_PER_FRAME * powf(1 + additionValueBase, 2);

		/*! ���x����ɓ��B�����瑬�x���ۂ߂� */
		speed = min(speed, MOVE_MAX_SPEED);

		/*! �ړ� */
		pos.x += speed;
	}
	/*! �ړ��I�� */
	else {
		index.x += 1;					/*!< �C���f�b�N�X�X�V */
		eMoveState = MoveState::WAIT;	/*!< �ړ���Ԃ�ҋ@�� */
		additionValueBase = 0;			/*!< ���Z�ʂ̏����� */
	}

}

/*!
	@brief	��ړ�
	@detail	�ړ��͓��������ړ�
*/
void Player::MoveUp()
{
	if (pos.y < nextPos.y) 
	{
		pos.y += MOVE_PER_FRAME;
	}
	else {
		/*! ���W�X�V */
		index.y -= 1;
		pos = nextPos;
		/*! �p�����[�^���Z�b�g */
		eMoveState = MoveState::WAIT;	/*!< �ړ���Ԃ�ҋ@�� */
		isInput = false;				/*!< ���͍ς݃t���O��߂� */
	}
}

/*!
	@brief	���ړ�
	@detail	�ړ��͓��������ړ�
*/
void Player::MoveDown()
{
	if (pos.y > nextPos.y)
	{
		pos.y -= MOVE_PER_FRAME;
	}
	else {
		/*! ���W�X�V */
		index.y += 1;
		pos = nextPos;
		/*! �p�����[�^���Z�b�g */
		eMoveState = MoveState::WAIT;	/*!< �ړ���Ԃ�ҋ@�� */
		isInput = false;				/*!< ���͍ς݃t���O��߂� */
	}
}

/*!
	@brief	�֐��̒x�����s
	@detail	���͌�A�j���[�V�������K��t���[���ɓ��B����܂ŏ�����҂�����
*/
void Player::Wait()
{
	/*! �֐����o�^����ĂȂ���Ώ������X�L�b�v */
	if (delayFunc != nullptr) {

		/*! �K��t���[���܂ŏ�����ۗ� */
		if (waitCount++ <= WAIT_FRAME) { return; }

		/*! ������ */
		delayFunc();
		delayFunc = nullptr;
		waitCount = 0;
	}
}

/*!
	@brief	����
*/
void Player::Input()
{
	/*****************************/
	/*		 �����͎�t��		 */
	/*****************************/
	
	/*! ���͍ς݂Ȃ珈���𔲂��� */
	if (isInput) { return; }

	/*! �ړ���ԂȂ珈���𔲂��� */
	if (eMoveState != MoveState::WAIT) { return; }

	/*****************************/
	/*			���ړ���		 */
	/*****************************/

	/*! �� */
	if (InputManager::GetInstance().GetLeft()) {
		dir = -1;/*!< �������� */
		const INDEX nextIndex = { index.x + dir, index.y };
		if (!pRefInfo->GetIsBlock(nextIndex)){
			eMoveState = MoveState::LEFT;
			Convert(&nextPos, nextIndex);
		}
	}
	/*! �E */
	if (InputManager::GetInstance().GetRight()) {
		dir = 1;/*!< �������E */
		const INDEX nextIndex = { index.x + dir, index.y };
		if (!pRefInfo->GetIsBlock(nextIndex)) {
			eMoveState = MoveState::RIGHT;
			Convert(&nextPos, nextIndex);
		}
	}

	/*! �v���C���[�̐��ʂɓ��������Ƃ��o����u���b�N
	��������΃A�N�V�����o���Ȃ��̂ŏ����𔲂��� */
	const INDEX front = { index.x + dir,index.y };

	if (!pRefInfo->GetIsMoveBlock(front)) { return; }

	/****************************/
	/*		���A�N�V������		*/
	/****************************/

	/*! �� */
	if (InputManager::GetInstance().GetUpActionDown()) {

		/*! �Ώۂ̃C���f�b�N�X���ړ��ł��邩�𔻒肵��ړ��ł��邩�𔻒� */
		const bool canMove =
			!pRefInfo->GetIsBlock({ index.x,index.y - 1 }) &&		/*!< �v���C���[�̏� */
			!pRefInfo->GetIsBlock({ index.x + dir,index.y + 1 });	/*!< �Ώۂ̉� */

		/*! �ړ���Ƀu���b�N�����݂��Ă����̂ŃA�N�V���������ɏ����𔲂��� */
		if (!canMove) { return; }

		/***************************************************/
		/*! �R�R�ɃA�j���[�V�����̍Đ������������Ă������� */
		/***************************************************/
		pModel->SetMotionNum(0);
		isInput = true;
		/***************************************************/

		/*! �x���֐��ɏ�����o�^���� */
		delayFunc = [&]{

			/*! �ړ����� */
			const INDEX nextIndex = { index.x, index.y - 1 };
			eMoveState = MoveState::UP;
			Convert(&nextPos, nextIndex);

			/*! �u���b�N�̈ړ����� */
			const INDEX bIndex = { index.x + dir,index.y };
			pRefInfo->UpAction(bIndex);

		};
	}

	/*! �� */
	if (InputManager::GetInstance().GetDownActionDown()) {

		/*! �Ώۂ̃C���f�b�N�X���ړ��ł��邩�𔻒肵��ړ��ł��邩�𔻒� */
		const bool canMove =
			!pRefInfo->GetIsBlock({ index.x,index.y + 1 }) &&		/*!< �v���C���[�̉� */
			!pRefInfo->GetIsBlock({ index.x + dir,index.y - 1 });	/*!< �Ώۂ̏� */

		/*! �ړ���Ƀu���b�N�����݂��Ă����̂ŃA�N�V���������ɏ����𔲂��� */
		if (!canMove) { return; }

		/***************************************************/
		/*! �R�R�ɃA�j���[�V�����̍Đ������������Ă������� */
		/***************************************************/
		pModel->SetMotionNum(0);
		isInput = true;
		/***************************************************/


		/*! �x���֐��ɏ�����o�^���� */
		delayFunc = [&] {

			/*! �ړ����� */
			const INDEX nextIndex = { index.x, index.y + 1 };
			eMoveState = MoveState::DOWN;
			Convert(&nextPos, nextIndex);

			/*! �u���b�N�̈ړ����� */
			const INDEX bIndex = { index.x + dir,index.y };
			pRefInfo->DownAction(bIndex);

		};
	}

}

/*!
	@brief	�C���f�b�N�X��XMFLOAT3�̍��W�ɕϊ�
*/
void Player::Convert(DirectX::XMFLOAT3 * outPos, INDEX index)
{
	outPos->x = LEFT_TOP_OFFSET.x + index.x * OFFSET.x;
	outPos->y = LEFT_TOP_OFFSET.y - index.y * OFFSET.y;
	outPos->z = LEFT_TOP_OFFSET.z;

}