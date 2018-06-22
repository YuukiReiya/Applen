#include "StageSelectScene.h"
#include "../../Fade/FadeManager.h"
#include "../../Camera/CameraManager.h"
#include "../../Input/InputManager/InputManager.h"
#include "../Title/TitleScene.h"
#include "../Main/MainScene.h"
#include "../../StageLoader/StageLoader.h"
#include <math.h>
#include "../../Sound/Storage/SoundStorage.h"

/*! 
	@brief	���O���
*/
using namespace DirectX;

/*!
	@brief	�萔�錾
*/
const int StageSelectScene::FADE_FRAME = 45;				/*!< �t�F�[�h������t���[�� */
const int StageSelectScene::STAGE_WIDTH_NUM = 5;			/*!< ��1�i�ɕ��ԃX�e�[�W�̐� */
const int StageSelectScene::STAGE_HEIGHT_NUM = 4;			/*!< �X�e�[�W�̒i�� */
const int StageSelectScene::CAMERA_MOVE_FRAME = 30;			/*!< �J�������ړ�������t���[�� */
const float StageSelectScene::SELECT_STAGE_UP_VALUE = 0.8f;	/*!< �I�𒆂̃X�e�[�W�̏㏸�� */

/*! �J�����̈ړ��I�t�Z�b�g */
const XMFLOAT3 StageSelectScene::CAMERA_MOVE_OFFSET = {
	0.0f,		/*! x */
	4.0f,		/*! y */
	0.0f,		/*! z */
};

/*!
	@brief	�ÓI�����o�ϐ��̐錾
*/
int StageSelectScene::selectNo = 1;/*!< �����l��1�Ƃ��A�X�e�[�W1��I����Ԃɂ��� */

/*!
	@brief	�R���X�g���N�^
*/
StageSelectScene::StageSelectScene()
{
}

/*!
	@brief	�f�X�g���N�^
*/
StageSelectScene::~StageSelectScene()
{
}

/*!
	@brief	������
*/
void StageSelectScene::Init()
{
	/*! �I�u�W�F�N�g�̎擾 */
	pSideObj = std::make_shared<TakeModelData>("ssSide");
	pMiddleObj = std::make_shared<TakeModelData>("ssMiddle");

	/*! �I�u�W�F�N�g�̒��ւ��p�e�N�X�`�� */
	pTexesTake = new TexesTake;
	pTexesTake->TexesCSVLoad("Resource/Model/uvStage/LoadUVPath.csv");

	/*! �p�����[�^������ */
	eState = SelectState::SELECT_STAGE;/*!< �X�e�[�W��I����Ԃ� */

	/*! ���[�v�� */
	const int NUM = STAGE_HEIGHT_NUM * STAGE_WIDTH_NUM;/*!< �I�u�W�F�N�g�̐� */

	/*! �I�u�W�F�N�g�̕\�����W�̎Z�o */
	objPos.clear();
	for (int i = 0; i < NUM; ++i) {

		const int x = i % STAGE_WIDTH_NUM;
		const int y = i / STAGE_WIDTH_NUM;

		/*!  */
		XMFLOAT3 pos;
		/*! (x,y)=(0,0)���W�{�I�t�Z�b�g���ԍ� */
		pos.x = -4 + 1.8f * static_cast<float>(x);
		pos.y = -0 + 3.6f * static_cast<float>(y);
		pos.z = 1;

		objPos.push_back(pos);
	}

	/*! �e�N�X�`�������� */
	Sprite load;
	auto sprites = load.InitTex("Resource/Texture/StageSelectTextures.csv");
	pSprites = std::move(sprites);	/*!< �e�N�X�����X�g�̓o�^ */

	/*! �J�����ݒ� */
	const int height = (selectNo - 1) / STAGE_WIDTH_NUM;	/*!< ���� */
	XMFLOAT3 fLook;/*!< �����_ */

	/*! ���_�ʒu�Z�o */
	fEyeFrom = {
		  0.0f + CAMERA_MOVE_OFFSET.x,
		  0.0f + height * CAMERA_MOVE_OFFSET.y,
		-10.0f + CAMERA_MOVE_OFFSET.z,
	};

	fLook = fEyeFrom;
	fLook.z = 0;		/*!< �␳ */
	fEyeTo = fEyeFrom;	/*!< �ړ�����W�̏����� */
	CameraManager::GetInstance().CameraSet(fEyeFrom, fLook);

	/*! BGM */
	auto bgm = SoundStorage::GetInstance().GetWave("STAGE_SELECT");
	bgm->Play(true);

	/*! �t�F�[�h�C���Ăяo�� */
	FadeManager::GetInstance().StartFadeInOnly(FADE_FRAME);
}

/*!
	@brief	�X�V
*/
SceneBase * StageSelectScene::Update(SceneManager * root)
{
	/*! �t�F�[�h���͏������Ȃ� */
	auto& fade = FadeManager::GetInstance();
	if (fade.IsFade()) { return this; }

	/*! �J�����̍��W�ƈړ�����r���Ĉ�v���Ă��Ȃ���Έړ����Ɣ��� */
	const bool misMatchX = fEyeFrom.x != fEyeTo.x;
	const bool misMatchY = fEyeFrom.y != fEyeTo.y;
	const bool misMatchZ = fEyeFrom.z != fEyeTo.z;
	const bool isMoving = misMatchX || misMatchY || misMatchZ;

	/*! �J�������ړ����Ă��� */
	if (isMoving) {
		CameraMove();	/*!< �J�����ړ����� */
		return this;
	}

	const bool canInput = (0b100 & eState) == 0;/*!< �I���\������ */

	/*! �I���ς݂Ȃ���͂��󂯕t���Ȃ� */
	if (canInput) {
		Input();
	}
	/*! �I���ςݏ��� */
	else {

		/*! BGM��~ */
		auto& sound = SoundStorage::GetInstance();
		sound.GetWave("STAGE_SELECT")->Stop();

		auto state = static_cast<SelectState>(0b11 & eState);
		switch (state)
		{
		case StageSelectScene::SELECT_STAGE:
			StageLoader::GetInstance().DataLoad(selectNo);
			return new MainScene;
		case StageSelectScene::RETURN_TTILE:
			return new TitleScene;
		}
	}
	return this;
}

/*!
	@brief	�`��
*/
void StageSelectScene::Render()
{
	/*! 1�p�X��:�V���h�E�}�b�v�̃����_�����O�X�R�[�v */
	{

	}

	/*! 2�p�X��:�m�[�}�������_�����O�X�R�[�v */
	DeviceManager::GetInstance().SetNormalRender();/*!< �`��N���A */
	{
		/*! �u���b�N�� */
		const int NUM = STAGE_HEIGHT_NUM * STAGE_WIDTH_NUM;

		/*! �^�C�g���I���� */
		const bool isTitle = (0b10 & eState) != 0;

		/************************************/
		/*		 ��I���X�e�[�W�̕`��		*/
		/************************************/

		/*! �X�e�[�W���[( 1 , 6 , 11 , 16 ) */
		for (int i = 1; i <= NUM; i += STAGE_WIDTH_NUM) {

			/*! �I���X�e�[�W */
			const bool isSelect = selectNo == i;	

			/*! �I���X�e�[�W�͕`�悵�Ȃ� */
			const bool isSkip = isSelect && !isTitle;
			if (isSkip) { continue; }

			/*! ���W�ƃe�N�X�`���������Ă���ϒ��z��̃C���f�b�N�X */
			const int index = i - 1;
			/*! �`�� */
			pSideObj->Get()->SetModelPos(objPos[index]);
			pSideObj->Get()->SetTexture(pTexesTake->GetTex(index));
			pSideObj->Render();
		}

		/*! �X�e�[�W�����Q( 2�`4 , 7�`9 , 12�`14 , 17�`19 ) */
		for (int i = 2; i <= NUM; ++i) {

			/*! ���[( 1 , 6 , 11 , 16 )�͏������X�L�b�v */
			if (i%STAGE_WIDTH_NUM == 1) {
				continue;
			}

			/*! �E�[( 5 , 10 , 15 , 20 )�͏������X�L�b�v */
			if (i%STAGE_WIDTH_NUM == 0) {
				/*! ���̂܂܃X�L�b�v���Ă����[�ɂȂ薳�ʂȃ��[�v�������邽�� i�����Z */
				++i; 
				continue;
			}

			/*! �I���X�e�[�W */
			const bool isSelect = selectNo == i;

			/*! �I���X�e�[�W�͕`�悵�Ȃ� */
			const bool isSkip = isSelect && !isTitle;
			if (isSkip) { continue; }

			/*! ���W�ƃe�N�X�`���������Ă���ϒ��z��̃C���f�b�N�X */
			const int index = i - 1;

			/*! �`�� */
			pMiddleObj->Get()->SetModelPos(objPos[index]);
			pMiddleObj->Get()->SetTexture(pTexesTake->GetTex(index));
			pMiddleObj->Render();
		}


		/*! �X�e�[�W�E�[( 5 , 10 , 15 , 20 ) */
		for (int i = STAGE_WIDTH_NUM; i <= NUM; i += STAGE_WIDTH_NUM) {
		

			/*! �I���X�e�[�W */
			const bool isSelect = selectNo == i;

			/*! �I���X�e�[�W�͕`�悵�Ȃ� */
			const bool isSkip = isSelect && !isTitle;
			if (isSkip) { continue; }

			/*! ���W�ƃe�N�X�`���������Ă���ϒ��z��̃C���f�b�N�X */
			const int index = i - 1;
			/*! �`�� */
			pSideObj->Get()->SetModelPos(objPos[index]);
			pSideObj->Get()->SetTexture(pTexesTake->GetTex(index));
			pSideObj->Render();
		}

		/************************************/
		/*		  �I���X�e�[�W�̕`��		*/
		/************************************/

		/*! �I�����Ă���X�e�[�W�����[�̃I�u�W�F�N�g */
		if (!isTitle) {
			const bool isLeftEnd	= selectNo % STAGE_WIDTH_NUM == 1;	/*!< ���[ */
			const bool isRightEnd	= selectNo % STAGE_WIDTH_NUM == 0;	/*!< �E�[ */
			const bool isBothEnd	= isLeftEnd && isRightEnd;			/*!< ���[ */
			const int index			= selectNo - 1;		/*!< �ϒ��z��̃C���f�b�N�X */

			/*! �I���I�u�W�F�N�g�� �[ �� �����̂ǂ���Ȃ̂����f */
			auto obj = isBothEnd ? pSideObj : pMiddleObj;
			
			/*! ���W�␳ */
			XMFLOAT3 pos = objPos[index];
			pos.y += SELECT_STAGE_UP_VALUE;

			/*! �`�� */
			obj->Get()->SetModelPos(pos);
			obj->Get()->SetTexture(pTexesTake->GetTex(index));
			obj->Render();
		}

		/*! ���S */
		pSprites["Logo"]->Render();

		/*! UI */
		if (isTitle) {
			pSprites["Title"]->Render();
		}
		else {
			pSprites["Title_None"]->Render();
		}
	}

}

/*!
	@brief	�j��
*/
void StageSelectScene::Destroy()
{
}

/*!
	@brief	����
*/
void StageSelectScene::Input()
{
	auto& input = InputManager::GetInstance();
	auto& sound = SoundStorage::GetInstance();

	/*****************************/
	/*		���J�[�\�����쁫	 */
	/*****************************/

	/*! �� */
	if (input.GetLeftDown()) {

		/*! �^�C�g���ʒu�ɃJ�[�\�������� */
		if (eState == SelectState::RETURN_TTILE) {

			/*! �E�[�̃X�e�[�W�̔ԍ��̎Z�o */
			const int y = (selectNo - 1) / STAGE_WIDTH_NUM;	

			/*! ���ݑI�����Ă���i�̉E�[�̔ԍ� */
			const int index = STAGE_WIDTH_NUM + y * STAGE_WIDTH_NUM;

			selectNo = index;
			eState = SelectState::SELECT_STAGE;
		}
		/*! �X�e�[�W�I�� */
		else {

			/*! ���[�̃X�e�[�W */
			if (selectNo%STAGE_WIDTH_NUM == 1) {
				eState = SelectState::RETURN_TTILE;
			}
			/*! ����ȊO */
			else {
				--selectNo;
			}
		}
		sound.GetWave("MOVE")->Play();
	}

	/*! �E */
	if (input.GetRightDown()) {

		/*! �^�C�g���ʒu�ɃJ�[�\�������� */
		if (eState == SelectState::RETURN_TTILE) {

			/*! ���[�̃X�e�[�W�̔ԍ��̎Z�o */
			const int y = (selectNo - 1) / STAGE_WIDTH_NUM;

			/*! ���ݑI�����Ă���i�̍��[�̔ԍ� */
			const int index = 1 + y * STAGE_WIDTH_NUM;

			selectNo = index;
			eState = SelectState::SELECT_STAGE;
		}
		/*! �X�e�[�W�I�� */
		else {

			/*! �E�[�̃X�e�[�W */
			if (selectNo%STAGE_WIDTH_NUM == 0) {
				eState = SelectState::RETURN_TTILE;
			}
			/*! ����ȊO */
			else {
				++selectNo;
			}
		}
		sound.GetWave("MOVE")->Play();
	}

	/*! �� */
	if (input.GetUpDown()) {

		/*! �ŏ�i�Ȃ珈�����X�L�b�v */
		const bool isSkip = (selectNo - 1) / STAGE_WIDTH_NUM == (STAGE_HEIGHT_NUM - 1);
		if (isSkip) { return; }

		fEyeTo = {
			fEyeFrom.x + CAMERA_MOVE_OFFSET.x,
			fEyeFrom.y + CAMERA_MOVE_OFFSET.y,
			fEyeFrom.z + CAMERA_MOVE_OFFSET.z,
		};
		sound.GetWave("MOVE")->Play();
	}

	/*! �� */
	if (input.GetUnderDown()) {

		/*! �ŉ��i�Ȃ珈�����X�L�b�v */
		const bool isSkip = (selectNo - 1) / STAGE_WIDTH_NUM == 0;
		if (isSkip) { return; }

		fEyeTo = {
			fEyeFrom.x - CAMERA_MOVE_OFFSET.x,
			fEyeFrom.y - CAMERA_MOVE_OFFSET.y,
			fEyeFrom.z - CAMERA_MOVE_OFFSET.z,
		};
		sound.GetWave("MOVE")->Play();
	}

	/*! ���� */
	if (input.GetDecideDown()) {

		sound.GetWave("ENTER")->Play();
		eState = static_cast<SelectState>(0b100 | eState);			/*!< �X�e�[�g������ */
		FadeManager::GetInstance().StartFadeOutOnly(FADE_FRAME);	/*!< �t�F�[�h�A�E�g�̊J�n */
	}
}

/*!
	@brief	�J�����̈ړ�(�c�ړ��̂�)
	@detail	�J�����̈ړ���ύX������������Input�֐��ƃR�R��ҏW����
*/
void StageSelectScene::CameraMove()
{
	/*! �������� */
	const int sign = fEyeFrom.y < fEyeTo.y ? 1 : -1;
	
	/*! �ړ��� */
	const float movePow = CAMERA_MOVE_OFFSET.y / CAMERA_MOVE_FRAME;

	/*! �ړ� */
	fEyeFrom.y += movePow * sign;
	
	/************************/
	/*		 �ړ��I��		*/
	/************************/

	/*! �{ */
	if (sign > 0) {

		if (fEyeFrom.y > fEyeTo.y) {

			fEyeFrom = fEyeTo;
			/*! �I���X�e�[�W����̒i�̂��̂ɂ��� */
			selectNo += STAGE_WIDTH_NUM;	
		}
	}
	/*! �| */
	else {

		if (fEyeFrom.y < fEyeTo.y) {

			fEyeFrom = fEyeTo;
			/*! �I���X�e�[�W�����̒i�̂��̂ɂ��� */
			selectNo -= STAGE_WIDTH_NUM;
		}
	}
	
	/*! ���W�X�V */
	XMFLOAT3 fLook = fEyeFrom;
	fLook.z = 0;/*!< �␳ */

	CameraManager::GetInstance().CameraSet(fEyeFrom, fLook);

}
