/*!
	@file	StageSelectScene.h
	@date	2018/05/14
	@author	�ԏ� �G�P
	@brief	�X�e�[�W�Z���N�g�V�[��
*/
#pragma once
#include "../Scene.h"
#include "../../Fbx/TakeModelData.h"
#include "../../Sprite/Sprite.h"
#include "../../Fbx/Texes/TexesTake.h"


class StageSelectScene :
	public Scene
{
public:
	StageSelectScene();
	~StageSelectScene();

	void Init()override;
	SceneBase* Update(SceneManager* root)override;
	void Render()override;
	void Destroy()override;

	/*! �I���X�e�[�W��ύX���� */
	static inline void SetSelectStage(int stage) {
		selectNo = stage;
	}
private:
	///*! �񋓑� */
	//enum InputState {
	//	STAGE_SELECTING,
	//	TITLE_SELECTING,
	//	STAGE_DECIDE,
	//	TITLE_DECIDE,
	//	UP_WAIT,
	//	DOWN_WAIT,
	//}eInputState;

	//void Input();

	//static const int STAGE_ROW_X;
	//static const int STAGE_ROW_Y;
	//static const float STAGE_SELECT_UP_VALUE;
	//static const float CAMERA_TRANSFORMATION_HEIGHT;

	//static const float BGM_WAIT_START_SECOND;
	//int bgmCount;
	//DirectX::XMFLOAT3 toCameraPos;
	//std::vector<DirectX::XMFLOAT3> ObjPos;

	//std::shared_ptr<TakeModelData> SideObj;		/*!< ���[(�P�ƂT)�̃X�e�[�W�Z���N�g�I�u�W�F�N�g */
	//std::shared_ptr<TakeModelData> MiddleObj;	/*!< �Q�`�S�܂ł̃X�e�[�W�Z���N�g�I�u�W�F�N�g */
	//uint8_t stageNo;
	//umap_str_sprite pSprites;
	//TexesTake* pTexesTake;
	//int moveFrame;
	//float movePower;

	////////////////////////////////////////

	/*! �I����� */
	enum SelectState :int8_t {
		/*!
		��3�������j���[�̑I����4���ڂ͌���ɔ���
		ex)
		010:"TITLE"
		��
		110:"TITLE"������
		*/
		SELECT_STAGE = 0b01,	/*!< �X�e�[�W */
		RETURN_TTILE = 0b10,	/*!< �^�C�g�� */
	}eState;

	/*! �萔 */
	static const int FADE_FRAME;				/*!< �t�F�[�h������t���[�� */
	static const int STAGE_WIDTH_NUM;			/*!< ��1�i�ɕ��ԃX�e�[�W�̐� */
	static const int STAGE_HEIGHT_NUM;			/*!< �c�ɕ��ԃX�e�[�W�̐� */
	static const int CAMERA_MOVE_FRAME;			/*!< �J�������ړ�������t���[�� */
	static const float SELECT_STAGE_UP_VALUE;	/*!< �I���X�e�[�W�̏㏸�� */

	static const DirectX::XMFLOAT3 CAMERA_MOVE_OFFSET;	/*!< �J�����̈ړ��I�t�Z�b�g */

	void Input();
	void CameraMove();

	/*! �ÓI�����o�ϐ� */
	static int selectNo;		/*!< �I�����Ă���X�e�[�W�̔ԍ� */

	/*! �����o�ϐ� */
	std::shared_ptr<TakeModelData> pSideObj;		/*!< ���[�̃u���b�N */
	std::shared_ptr<TakeModelData> pMiddleObj;	/*!< ���[�������������̃u���b�N */
	TexesTake* pTexesTake;						/*!< �u���b�N�̕ύX�pUV */
	umap_str_sprite pSprites;					/*!< �X�v���C�g */
	std::vector<DirectX::XMFLOAT3>objPos;		/*!< �X�e�[�W�I�u�W�F�N�g�̕\�����W */

	DirectX::XMFLOAT3 fEyeFrom;					/*!< ���_�ʒu */
	DirectX::XMFLOAT3 fEyeTo;					/*!<  �ړ�����W�̎��_�ʒu */
};

