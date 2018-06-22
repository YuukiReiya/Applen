/*!
	@file	Player.h
	@date	2018/04/04
	@author	�ԏ� �G�P
	@brief	�v���C���[
*/
#pragma once
#include "../../Fbx/PlayerModel/PlayerModel.h"
#include "../../Math/Index.h"
#include <functional>

/*!
	@def	�O���錾
*/
class SquaresManager;

/*! 
	@brief	�v���C���[�N���X
*/
class Player
{
public:
	/*! �R���X�g���N�^���f�X�g���N�^ */
	Player();
	~Player();

	DirectX::XMFLOAT2 OFFSET;

	/*! ���J�֐� */
	void Initialize();
	void Update();
	void Render();
	void Finalize();

	/*! �Z�b�^�[ */
	void SetRef(SquaresManager* pRef) { pRefInfo = pRef; }
	void SetLoadData();
	/*! �Q�b�^�[ */
	INDEX GetIndex()const { return index; }
private:
	void Move();
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();
	void Wait();
	void Input();
	void Convert(DirectX::XMFLOAT3* outPos,INDEX index);

	/*! �񋓑� */
	enum MoveState {
		WAIT,
		LEFT,
		RIGHT,
		UP,
		DOWN,
	}eMoveState;

	/*! �萔 */
	static const DirectX::XMFLOAT3 LEFT_TOP_OFFSET;
	static const DirectX::XMFLOAT3 MODEL_ROTATION;
	static const float  MOVE_PER_FRAME;
	static const float	MOVE_MAX_SPEED;
	static const float  SPEED_ADD_VALUE;
	static const int WAIT_FRAME;

	/*! �p�����[�^ */
	PlayerModel*		pModel;				/*!< ���f�� */
	DirectX::XMFLOAT3	pos;				/*!< ���W */
	DirectX::XMFLOAT3	rot;				/*!< ��] */
	DirectX::XMFLOAT3	scale;				/*!< �X�P�[�� */
	int					dir;				/*!< ���� */
	INDEX				index;				/*!< �C���f�b�N�X */
	DirectX::XMFLOAT3	nextPos;			/*!< �ړ�����W */
	float				speed;				/*!< ���x */
	float				additionValueBase;	/*!< ���x���Z�ɗp�����b�� */
	int					waitCount;			/*!< �ҋ@�J�E���^ */
	bool				isInput;			/*!< ���͍ς݃t���O */

	/*! �Q�� */
	SquaresManager*		pRefInfo;			/*!< �}�X�Ǘ��̃|�C���^ */

	/*! �x���֐� */
	std::function<void()>delayFunc;			/*!< �x���֐� */
};