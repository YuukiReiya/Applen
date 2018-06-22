#include "TitleScene.h"
#include "../../Device/DeviceManager.h"
#include "../../Input/InputManager/InputManager.h"
#include "../../Fade/FadeManager.h"
#include "../StageSelect/StageSelectScene.h"
#include "../../DataManager/DataManager.h"
#include "../../Sound/Storage/SoundStorage.h"


/*!
	@def	�萔�錾
*/
const int TitleScene::FADE_FRAME = 45;/*!< �t�F�[�h������t���[�� */

/*!
	@brief	���O���
*/
using namespace DirectX;
using namespace std;

/*!
	@brief	�R���X�g���N�^
*/
TitleScene::TitleScene()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* ������
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void TitleScene::Init()
{
	/*! �e�N�X�`�������� */
	Sprite textures;
	std::string path = "Resource/Texture/TitleTextures.csv";
	pSprites = std::move(textures.InitTex(path.c_str()));

	pTbg = new TitleBackGround;
	pTbg->Init();
		
	/*! �p�����[�^������ */
	DataManager::GetInstance().Load();					/*!< �f�[�^�ǂݍ��� */
	FadeManager::GetInstance().StartFadeInOnly(FADE_FRAME);		/*!< �t�F�[�h�J�n */
	
	eMenu = TitleMenu::TITLE_PLAY;						/*!< �I�����j���[ */
	pState = new PlayState(this);						/*!< �I���X�e�[�g */

	/*! �J�����ݒ� */
	XMFLOAT3 fEye	= { 0,0,-10.0f };
	CameraManager::GetInstance().CameraSet(fEye);

	/*! BGM */
	auto bgm = SoundStorage::GetInstance().GetWave("TITLE");
	bgm->Play(true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* �X�V
* @param (root) ���z�֐���̈���(���ۂɎg���͂��Ȃ�)
* @return (next) �����𖞂����Ă���Ύ��̃V�[����Ԃ�
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
SceneBase* TitleScene::Update(SceneManager* root)
{
	/*! �t�F�[�h���͍X�V���Ȃ� */
	if (FadeManager::GetInstance().IsFade()) { return this; }

	/*! ���j���[�����:true */
	const bool isDecided = (0b1000 & eMenu) != 0;

	/*! ���肳��Ă�Γ��͎͂󂯕t���Ȃ� */
	if (!isDecided) {
		Input();
		return this;
	}

	/*! ���肳�ꂽ�X�e�[�g�̍X�V */
	pState->Update();

	/*! ���V�[���𔻕� */
	SceneBase* ret = pState->next != nullptr ? pState->next : this;

	return ret;

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* �`��
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void TitleScene::Render()
{
	DeviceManager::GetInstance().SetNormalRender();
	auto menu = static_cast<TitleMenu>(0b0111 & eMenu);

	auto play		= pSprites["Play_None"];
	auto credit		= pSprites["Credit_None"];
	auto exit		= pSprites["Exit_None"];

	pTbg->Render();

	/*! ���S */
	pSprites["Logo"]->Render();

	/*! �I�𒆃��j���[�̃X�v���C�g��ς��� */
	switch (menu)
	{
	case TitleScene::TITLE_PLAY:
		play = pSprites["Play"];
		break;
	case TitleScene::TITLE_CREDIT:
		credit = pSprites["Credit"];
		break;
	case TitleScene::TITLE_EXIT:
		exit = pSprites["Exit"];
		break;
	default:
		assert("Invalid value");
		break;
	}


	play->Render();
	credit->Render();
	exit->Render();


	if (menu == TitleMenu::TITLE_CREDIT) {
		pState->Render();
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* ���
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void TitleScene::Destroy()
{
}

/*! 
	@brief	����
*/

void TitleScene::Input()
{
	auto& input = InputManager::GetInstance();
	auto& sound = SoundStorage::GetInstance();

	/*****************************/
	/*		���J�[�\�����쁫	 */
	/*****************************/

	/*! �� */
	if (input.GetUpDown()) {

		if (eMenu != TitleMenu::TITLE_PLAY) {
			eMenu = static_cast<TitleMenu>(eMenu >> 1);
		}
		else {
			eMenu = TitleMenu::TITLE_EXIT;
		}
		sound.GetWave("MOVE")->Play();
	}

	/*! �� */
	if (input.GetUnderDown()) {

		if (eMenu != TitleMenu::TITLE_EXIT) {
			eMenu = static_cast<TitleMenu>(eMenu << 1);
		}
		else {
			eMenu = TitleMenu::TITLE_PLAY;
		}
		sound.GetWave("MOVE")->Play();
	}

	/*! ���� */
	if (input.GetDecideDown()) {
		eMenu = static_cast<TitleMenu>(0b1000 | eMenu);
		sound.GetWave("ENTER")->Play();
	}

	/*! ���j���[�����:true */
	const bool isDecided = (0b1000 & eMenu) != 0;

	/*! ���肳��Ă�Ώ������X�L�b�v */
	if (!isDecided) { return; }

	/*! ���肳�ꂽ�u�Ԃ̏��� */

	auto menu = static_cast<TitleMenu>(0b0111 & eMenu);

	auto& fade = FadeManager::GetInstance();	

	switch (menu)
	{
		/*! �Q�[���v���C */
	case TitleScene::TITLE_PLAY:
	{
		fade.StartFadeOutOnly(FADE_FRAME);
		pState = new PlayState(this);
		StageSelectScene::SetSelectStage(1);/*!< �I���X�e�[�W��1�Ԗڂɂ��� */
	}
		break;
		/*! �N���W�b�g */
	case TitleScene::TITLE_CREDIT:
	{
		pState = new CreditState(this);
		pState->Initialize();
	}
		break;
		/*! �Q�[���I�� */
	case TitleScene::TITLE_EXIT:
	{
		fade.StartFadeOutOnly(FADE_FRAME);
		pState = new ExitState(this);
	}
		break;
	default:
		assert("Invalid value");
		break;
	}


}

/********************************************************/
/*			�ȉ��Ƀ^�C�g���V�[���̊e�X�e�[�g�̒�`		*/
/********************************************************/

/********************/
/*		"PLAY"		*/
/********************/

/*!
	@brief	������
*/
void TitleScene::PlayState::Initialize()
{
}

/*!
	@brief	�j��
*/
void TitleScene::PlayState::Finalize()
{
}

/*!
	@brief	�X�V
*/
void TitleScene::PlayState::Update()
{
	auto bgm = SoundStorage::GetInstance().GetWave("TITLE");
	bgm->Stop();	/*!< BGM��~ */

	next = new StageSelectScene;
}

/*!
	@brief	�`��
*/
void TitleScene::PlayState::Render()
{
}

/********************/
/*	   "CREDIT"  	*/
/********************/

/*!
	@brief	�萔�錾
*/
const string TitleScene::CreditState::HASH_NAME = "CreditURL";	/*!< �\������e�N�X�`���̃n�b�V���� */
const int TitleScene::CreditState::SCALE_FRAME = 50;			/*!< �g��k���ɂ�����t���[�� */

/*! �X�P�[���ω��ɔ����ω�������W�̃I�t�Z�b�g */
const XMFLOAT2 TitleScene::CreditState::POSITION_OFFSET = {
	-2.0f / SCALE_FRAME,	/*!< x */
	-1.0f / SCALE_FRAME,	/*!< y */
};

/*! �e�N�X�`���̃X�P�[�� */
const XMFLOAT2 TitleScene::CreditState::TEXTURE_SCALE = {
	0.2f,		/*!< x */
	0.2f,		/*!< y */
};


/*!
	@brief	������
*/

void TitleScene::CreditState::Initialize()
{
	/*! �e�N�X�`���̃|�W�V�����ƃX�P�[�����[���ɂ��� */
	scene->pSprites[HASH_NAME]->GetTexData()->xfPos ={ 0, 0, -6 };
	scene->pSprites[HASH_NAME]->SetScale({ 0,0 });
	/*! �J�E���^�����Z�b�g */
	count = 0;
	isOpen = true;/*!< �����̓N���W�b�g���J����� */
}

/*!
	@brief	�j��
*/
void TitleScene::CreditState::Finalize()
{
}

/*!
	@brief	�X�V
*/
void TitleScene::CreditState::Update()
{
	/*! �t���[�����J�E���g */
	++count;

	/*! �X�P�[�����O */
	if (count <= SCALE_FRAME) {
		Scaling();
	}
	else {

		/*! �X�P�[�����O�J�n���� */
		if (InputManager::GetInstance().GetDecideDown()) {
			count = 0;
			SoundStorage::GetInstance().GetWave("ENTER")->Play();
		}
	}

	/*! �X�P�[�����O���I������u�� */
	if (count == SCALE_FRAME) {

		/*! ���I������烁�j���[�̓��͍ς݂������� */
		if (!isOpen) {
			scene->eMenu = static_cast<TitleMenu>(0b0111 & scene->eMenu);
		}

		isOpen = !isOpen;/*!< �t�̏�Ԃ��N����悤�ɂ��� */
	}
}

/*!
	@brief	�`��
*/
void TitleScene::CreditState::Render()
{
	scene->pSprites[HASH_NAME]->Render();
}

/*!
	@brief	�X�P�[�����O�������s��
*/
void TitleScene::CreditState::Scaling()
{
	/*! �X�P�[���̉��Z�� */
	XMFLOAT2 add = {
		TEXTURE_SCALE.x / SCALE_FRAME,
		TEXTURE_SCALE.y / SCALE_FRAME,
	};

	auto sprite = scene->pSprites[HASH_NAME];
	auto texture = scene->pSprites[HASH_NAME]->GetTexData();

	/*! �����𔻒肷�� */
	int sign = isOpen ? 1 : -1;		/*!< OPEN:1,CLOSE:-1 */

	/*! �X�P�[���̕ϊ����� */
	XMFLOAT2 scale = sprite->GetScale();
	scale.x += add.x*sign;
	scale.y += add.y*sign;

	sprite->SetScale(scale);

	/*! ���W��␳���� */
	XMFLOAT2 pos = sprite->GetPos2D();
	pos.x += POSITION_OFFSET.x*sign;
	pos.y += POSITION_OFFSET.y*sign;

	sprite->SetPos(pos);

}

/********************/
/*		"EXIT"		*/
/********************/

/*!
	@brief	������
*/
void TitleScene::ExitState::Initialize()
{
}

/*!
	@brief	�j��
*/
void TitleScene::ExitState::Finalize()
{
}

/*!
	@brief	�X�V
*/
void TitleScene::ExitState::Update()
{
	auto bgm = SoundStorage::GetInstance().GetWave("TITLE");
	bgm->Stop();	/*!< BGM��~ */

	exit(NULL);
}

/*!
	@brief	�`��
*/
void TitleScene::ExitState::Render()
{
}
