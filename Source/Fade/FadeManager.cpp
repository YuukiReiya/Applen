#include "FadeManager.h"
#include "../Input/InputManager/InputManager.h"



FadeManager::FadeManager()
{
	count = fadeFrame = 0;
	addVal = 0.0f;
}


FadeManager::~FadeManager()
{
}

/*!
	@brief	������
*/
void FadeManager::Initialize()
{
	pSprite = new SpriteBase;
	pTexData = new TEXTURE_DATA;

	/*! �e�N�X�`�������� */
	strcpy_s(pTexData->filePath, "Resource/Texture/Fade.png");
	pTexData->xfPos = { -10,-6,0.1f };
	pTexData->xfRot = { 0,0,0 };
	pTexData->xfScale = { 10,10,1 };
	pTexData->xfSize = { 1980,1080 };
	pTexData->xfColor = { 0,0,0,0 };
	pTexData->isFollow = true;
	pSprite->Init(pTexData);

	eState = State::NONE;
}

/*!
	@brief	�X�V
*/
void FadeManager::Update()
{
	/*! �X�V�̕K�v��������Ώ������Ȃ� */
	if (eState == State::NONE) { return; }

	/*! ���l�̉��Z�E���Z */
	pTexData->xfColor.w += addVal;

	/*! �t�F�[�h�I������ */
	bool isEnd = false;

	/*! �t�F�[�h�C�����̏��� */
	if (eState == State::FADE_IN) {

		/*! 0�ȉ��ɂȂ����� 0 �ɂ��� */
		if (pTexData->xfColor.w <= 0) {
			pTexData->xfColor.w = 0;
			isEnd = true;
		}

	}
	/*! �t�F�[�h�A�E�g���̏��� */
	else if (eState == State::FADE_OUT) {

		/*! 1�ȏ�ɂȂ����� 1 �ɂ��� */
		if (pTexData->xfColor.w >= 1) {
			pTexData->xfColor.w = 1;
			isEnd = true;
		}

	}

	/*! �t�F�[�h�I���܂Ń��^�[�� */
	if (!isEnd) { return; }

	/*! ��Ԃ̏����� */
	eState = State::NONE;
	count = fadeFrame = 0;
	addVal = 0.0f;
}

/*!
	@brief		�t�F�[�h�C���J�n
	@param[in]	�t�F�[�h�t���[��
*/
void FadeManager::StartFadeInOnly(int frame)
{
	eState = State::FADE_IN;
	addVal = -(1 / static_cast<float>(frame));
}

/*!
	@brief		�t�F�[�h�A�E�g�J�n
	@param[in]	�t�F�[�h�t���[��
*/
void FadeManager::StartFadeOutOnly(int frame)
{
	eState = State::FADE_OUT;
	addVal = 1 / static_cast<float>(frame);
}


/*!
	@brief	�`��
*/
void FadeManager::Render()
{
	pSprite->Render();
}
