#include "MainScene.h"
#include "../Title/TitleScene.h"
#include "../StageSelect/StageSelectScene.h"
#include "../../Device/DeviceManager.h"
#include "../../StageLoader/StageLoader.h"
#include "../../Input/InputManager/InputManager.h"
#include "../../Fade/FadeManager.h"
#include "../../DataManager/DataManager.h"	
#include "../../Sound/Storage/SoundStorage.h"

/*!
	@brief	���O���
*/
using namespace DirectX;

/*!
	@def	�萔�錾
*/
const int MainScene::FADE_FRAME = 45;		/*!< �t�F�[�h�Ɋ|����t���[�� */

/*!
	@brief	�R���X�g���N�^
*/
MainScene::MainScene()
{
}

/*!
	@brief	�f�X�g���N�^
*/
MainScene::~MainScene()
{
}

/*!
	@brief	������
*/
void MainScene::Init()
{
	/*! �C���X�^���X���� */
	{
		pSquareManager	= new SquaresManager;
		pStageManager	= new StageManager;
		pPlayer			= new Player;
		pBack			= new BackGround;
	}

	/*! ������ */
	{
		/*! �v���C���[ */
		pPlayer->Initialize();

		/*! �}�l�[�W���[�̏����� */
		pSquareManager->Initialize();
		pStageManager->Initialize();

		/*! ���[�h�f�[�^�ǂݍ��� */
		pPlayer->SetLoadData();
		pStageManager->SetLoadData();

		/*! �Q�Ɛ�ݒ� */
		pSquareManager->SetRef(pPlayer, pStageManager);
		pStageManager->SetRefData(pSquareManager);
		pPlayer->SetRef(pSquareManager);

		/*! �J���� */
		XMFLOAT3 eye, look;
		eye = StageLoader::GetInstance().GetCameraInfo().pos;
		look = eye;
		look.z = 0;
		CameraManager::GetInstance().CameraSet(eye, look);

		/*! �e�N�X�`�� */
		Sprite sprite;
		std::string path;
		path = "Resource/Texture/GameTextures.csv";
		pSprites = std::move(sprite.InitTex(path.c_str()));

		/*! �w�i */
		pBack->Init();

		/*! �X�e�[�g */
		pState = new GameState(this);
	}

	/*! �X�e�[�W�Z���N�g�̃J�[�\�������݂̃X�e�[�W�ʒu�ɍ��킹�� */
	int stage = StageLoader::GetInstance().GetLoadedStageNo();
	StageSelectScene::SetSelectStage(stage);

	/*! BGM�Đ� */
	StartBGM();

	/*! �t�F�[�h�C���Ăяo�� */
	FadeManager::GetInstance().StartFadeInOnly(FADE_FRAME);
}

/*!
	@brief	�X�V
*/
#include "../../Input/Keyboard/Keyboard.h"
SceneBase * MainScene::Update(SceneManager * root)
{
	/*! �t�F�[�h���͍X�V���Ȃ� */
	if (FadeManager::GetInstance().IsFade()) { return this; }

	/*! �X�e�[�g�̍X�V */
	pState = pState->Update();


	//�f�o�b�O
	if (Keyboard::GetButtonDown(Keyboard::SHIFT)) {
		StageLoader::GetInstance().DataLoad(StageLoader::GetInstance().GetLoadedStageNo());
		Init();
	}

	/*! ���V�[���𔻕� */
	SceneBase*ret = pState->next != nullptr ? pState->next : this;

	return ret;
}

/*!
	@brief	�`��
*/
void MainScene::Render()
{
	/*! 1�p�X��:�V���h�E�}�b�v�̃����_�����O�X�R�[�v */
	{
	
	}

	/*! 2�p�X��:�m�[�}�������_�����O�X�R�[�v */
	DeviceManager::GetInstance().SetNormalRender();/*!< �`��N���A */
	{
		pBack->Render();
		pState->Render();
	}
}

/*!
	@brief	�j��
*/
void MainScene::Destroy()
{
	/*! ��� */
	SAFE_DELETE(pState);
	SAFE_DELETE(pPlayer);
	SAFE_DELETE(pStageManager);
	SAFE_DELETE(pSquareManager);
}

/*!
	@brief	�X�e�[�W�ɉ�����BGM���Đ�
*/
void MainScene::StartBGM()
{
	std::string soundHash = "FLOOR";
	int no = StageLoader::GetInstance().GetLoadedStageNo();

	if (no <= 5)		{ no = 1; }
	else if (no <= 10)	{ no = 2; }
	else if (no <= 15)	{ no = 3; }
	else				{ no = 4; }
	soundHash += std::to_string(no);
	SoundStorage::GetInstance().GetWave(soundHash)->Play(true);
}

/*!  
	@brief	�Đ�����BGM�̒�~
*/
void MainScene::StopBGM()
{
	std::string soundHash = "FLOOR";
	int no = StageLoader::GetInstance().GetLoadedStageNo();

	if (no <= 5)		{ no = 1; }
	else if (no <= 10)	{ no = 2; }
	else if (no <= 15)	{ no = 3; }
	else				{ no = 4; }
	soundHash += std::to_string(no);
	SoundStorage::GetInstance().GetWave(soundHash)->Stop();
}

/********************************************************/
/*			�ȉ��ɃQ�[���V�[���̊e�X�e�[�g�̒�`		*/
/********************************************************/

/********************/
/*		�Q�[��		*/
/********************/

/*!
	@brief	������
*/
void MainScene::GameState::Initialize()
{
}

/*!
	@brief	�j��
*/
void MainScene::GameState::Finalize()
{
}

/*!
	@brief	�X�V
*/
MainScene::StateBase * MainScene::GameState::Update()
{
	auto& player	= main->pPlayer;
	auto& stage		= main->pStageManager;
	auto& square	= main->pSquareManager;
	auto& input		= InputManager::GetInstance();
	auto& sound		= SoundStorage::GetInstance();

	/*! �|�[�Y */
	if (input.GetStartDown()) {
		auto pause = new PauseState(main);
		pause->Initialize();
		sound.GetWave("OPEN")->Play();
		return pause;
	}

	/*! �N���A */
	if (square->GetIsClear()) {
		auto clear = new ClearState(main);
		main->StopBGM();
		sound.GetWave("CLEAR_SE")->Play();
		clear->Initialize();
		return clear;
	}

	/*! �Q�[�����̍X�V���� */
	player->Update();	/*!< �v���C���[ */
	stage->Update();	/*!< �X�e�[�W */
	square->Update();	/*!< �}�X�Ǘ� */

	return this;
}

/*!
	@brief	�`��
*/
void MainScene::GameState::Render()
{
	main->pPlayer->Render();
	main->pStageManager->Render();
#ifdef DebugDraw
	main->pSquareManager->Render();
#endif // DebugDraw


}

/********************/
/*		�|�[�Y		*/
/********************/

/*!
	@brief	������
*/
void MainScene::PauseState::Initialize()
{

	eMenu = PauseMenu::PAUSE_PLAY;		/*!< �J�[�\�������ʒu�̓v���C */
}

/*!
	@brief	�j��
*/
void MainScene::PauseState::Finalize()
{
}

/*! 
	@brief	�X�V
*/
MainScene::StateBase * MainScene::PauseState::Update()
{
	/*****************************/
	/*		 �����͎�t��		 */
	/*****************************/
	
	/*!< ���j���[�����:true */
	const bool isDecided = (0b00010000 & eMenu) != 0;	

	/*! ���肳��Ă�Γ��͎͂󂯕t���Ȃ� */
	if (!isDecided) {
		Input();		/*!< ���͏��� */
		return this;
	}

	/*****************************************/
	/*		 �����j���[�����̏�����		 */
	/*****************************************/

	/*! �ȍ~�̏����̓��j���[�����肳�ꂽ����� */
	PauseMenu menu = static_cast<PauseMenu>(0b00001111 & eMenu);

	switch (menu)
	{
	case MainScene::PauseState::PAUSE_PLAY:
		return new GameState(main);
	case MainScene::PauseState::PAUSE_RETRY:
		main->Init();
		return new GameState(main);
	case MainScene::PauseState::PAUSE_STAGESELECT:
		next = new StageSelectScene;
		break;
	case MainScene::PauseState::PAUSE_TITLE:
		next = new TitleScene;
		break;
	default:
		assert("Invalid value");
		break;
	}

	return this;
}

/*!
	@brief	�`��
*/
void MainScene::PauseState::Render()
{
	/*! �v���C���[�ƃX�e�[�W�̕`��͏�ɍs�� */
	main->pPlayer->Render();
	main->pStageManager->Render();

	/*! ���j���[������ */
	auto play  = main->pSprites["pPlay_None"];
	auto retry = main->pSprites["pRetry_None"];
	auto stage = main->pSprites["pStage_None"];
	auto title = main->pSprites["pTitle_None"];

	PauseMenu menu = static_cast<PauseMenu>(0b00001111 & eMenu);

	switch (menu)
	{
	case MainScene::PauseState::PAUSE_PLAY:
		play = main->pSprites["pPlay"];
		break;
	case MainScene::PauseState::PAUSE_RETRY:
		retry= main->pSprites["pRetry"];
		break;
	case MainScene::PauseState::PAUSE_STAGESELECT:
		stage = main->pSprites["pStage"];
		break;
	case MainScene::PauseState::PAUSE_TITLE:
		title = main->pSprites["pTitle"];
		break;
	default:
		assert("Invalid value");
		break;
	}

	/*! ���̃t�F�[�h */
	main->pSprites["pBack"]->Render();

	/*! �e���j���[ */
	play->Render();
	retry->Render();
	stage->Render();
	title->Render();

}

/*!
	@brief	����
*/
void MainScene::PauseState::Input()
{
	auto& input = InputManager::GetInstance();
	auto& sound = SoundStorage::GetInstance();

	/*****************************/
	/*		���J�[�\�����쁫	 */
	/*****************************/

	/*! �� */
	if (input.GetLeftDown()) {

		if (eMenu != PauseMenu::PAUSE_PLAY) {
			eMenu = static_cast<PauseMenu>(eMenu >> 1);
		}
		else {
			eMenu = PauseMenu::PAUSE_TITLE;
		}
		sound.GetWave("MOVE")->Play();
	}

	/*! �E */
	if (input.GetRightDown()) {

		if (eMenu != PauseMenu::PAUSE_TITLE) {
			eMenu = static_cast<PauseMenu>(eMenu << 1);
		}
		else {
			eMenu = PauseMenu::PAUSE_PLAY;
		}
		sound.GetWave("MOVE")->Play();
	}

	/*! ���� */
	if (input.GetDecideDown()) {
		sound.GetWave("ENTER")->Play();
		eMenu = static_cast<PauseMenu>(0b00010000 | eMenu);
	}

	/*! START�{�^���������ꂽ */
	if (input.GetStartDown()) {
		sound.GetWave("CLOSE")->Play();
		eMenu = static_cast<PauseMenu>(0b00010000 | PauseMenu::PAUSE_PLAY);
	}

	/*****************************************/
	/*! �����肳��ĂȂ���Ώ������X�L�b�v�� */
	/*****************************************/
	const bool isSkip = (0b00010000 & eMenu) == 0;
	if (isSkip) { return; }/*!< ���肳��ĂȂ��̂ŏ����𔲂��� */

	
	/*****************************************/
	/*!		���ȍ~�͌��肳�ꂽ�u�Ԃ̏�����	 */
	/*****************************************/
	PauseMenu menu = static_cast<PauseMenu>(0b00001111 & eMenu);
	
	/*! "Play"��I�񂾎��ȊO�̓t�F�[�h��������  */
	if (menu != PauseMenu::PAUSE_PLAY) {
		main->StopBGM();
		FadeManager::GetInstance().StartFadeOutOnly(FADE_FRAME);
	}

}

/********************/
/*		�N���A		*/
/********************/

/*!
	@def	�萔�錾
*/
const int MainScene::ClearState::WAIT_FRAME = 240;/*!< ���̃t���[�����ҋ@���Ă���N���ABGM�𗬂� */

/*!
	@brief	������
*/
void MainScene::ClearState::Initialize()
{
	isLastStage = main->pSquareManager->GetIsLastStage();
	count = 0;

	/*! �ŏI�X�e�[�W������ */
	if (isLastStage) {
		eMenu = ClearMenu::CLEAR_STAGESELECT;	/*!< �ŏI�X�e�[�W�̏ꍇ"STAGE_SELECT"��"TITLE"�̓���Ȃ̂�STAGE_SELECT������ */
	}
	else {
		eMenu = ClearMenu::CLEAR_NEXTSTAGE;
	}
}

/*!
	@brief	�j��
*/
void MainScene::ClearState::Finalize()
{
}

/*!
	@brief	�X�V
*/
MainScene::StateBase * MainScene::ClearState::Update()
{
	/*****************************/
	/*		�@��SE�ҋ@��		 */
	/*****************************/
	auto& sound = SoundStorage::GetInstance();

	if (++count == WAIT_FRAME) {
		sound.GetWave("STAGE_CLEAR")->Play(true);
	}

	/*****************************/
	/*		 �����͎�t��		 */
	/*****************************/

	/*! ���j���[�����:true */
	const bool isDecided = (0b00001000 & eMenu) != 0;

	/*! ���肳��Ă�Γ��͎͂󂯕t���Ȃ� */
	if (!isDecided) {
		Input();		/*!< ���͏��� */
		return this;
	}

	/*****************************************/
	/*		 �����j���[�����̏�����		 */
	/*****************************************/

	/*! �ȍ~�̏����̓��j���[�����肳�ꂽ����� */
	ClearMenu menu = static_cast<ClearMenu>(0b00000111 & eMenu);

	switch (menu)
	{
	case MainScene::ClearState::CLEAR_NEXTSTAGE:
		main->StopBGM();
		main->Init();
		return new GameState(main);
	case MainScene::ClearState::CLEAR_STAGESELECT:
		next = new StageSelectScene;
		main->StopBGM();
		sound.GetWave("STAGE_CLEAR")->Stop();
		break;
	case MainScene::ClearState::CLEAR_TITLE:
		next = new TitleScene;
		main->StopBGM();
		sound.GetWave("STAGE_CLEAR")->Stop();
		break;
	default:
		assert("Invalid value");
		break;
	}

	return this;
}

/*!
	@brief	�`��
*/
void MainScene::ClearState::Render()
{
	/*! �v���C���[�ƃX�e�[�W�̕`��͏�ɍs�� */
	main->pPlayer->Render();
	main->pStageManager->Render();


	/*! ���j���[������ */
	auto next  = main->pSprites["cNextStage_None"];
	auto stage = main->pSprites["cStageSelect_None"];
	auto title = main->pSprites["cTitle_None"];

	/*! �ŏI�X�e�[�W�̏ꍇ�g���e�N�X�`�����ς�� */
	if (isLastStage) {

		/*!< "NEXT STAGE"�������̂ŌĂяo�����ۃG���[���ł�悤��nullptr��n���Ă��� */
		next = nullptr;
		/*! �ǂݍ��ރe�N�X�`�����ŏI�X�e�[�W�p�ɐ؂�ւ��� */
		stage = main->pSprites["cLastStage_None"];
		title = main->pSprites["cLastTitle_None"];

	}

	auto menu = static_cast<ClearMenu>(0b00000111 & eMenu);

	switch (menu)
	{
	case MainScene::ClearState::CLEAR_NEXTSTAGE:
		next = main->pSprites["cNextStage"];
		break;
	case MainScene::ClearState::CLEAR_STAGESELECT:
	{
		if (isLastStage) {
			stage = main->pSprites["cLastStage"];
		}
		else {
			stage= main->pSprites["cStageSelect"];
		}
	}
		break;
	case MainScene::ClearState::CLEAR_TITLE:
	{
		if (isLastStage) {
			title = main->pSprites["cLastTitle"];
		}
		else {
			title= main->pSprites["cTitle"];
		}
	}
		break;
	default:
		assert("Invalid value");
		break;
	}

	/*! �N���A���� */
	main->pSprites["cClear"]->Render();

	/*! �e���j���[ */
	if (next != nullptr) {
		next->Render();
	}
	stage->Render();
	title->Render();
}

/*!
	@brief	����
*/
void MainScene::ClearState::Input()
{
	auto& input = InputManager::GetInstance();
	auto& sound = SoundStorage::GetInstance();

	/*****************************/
	/*		���J�[�\�����쁫	 */
	/*****************************/

	/*! �� */
	if (input.GetLeftDown()) {

		/*! �ŏI�X�e�[�W��"NEXT STAGE"�������̂ŕ\�����Ȃ����� */
		if (isLastStage) {

			if (eMenu == ClearMenu::CLEAR_STAGESELECT) {
				eMenu = ClearMenu::CLEAR_TITLE;
			}
			else if (eMenu == ClearMenu::CLEAR_TITLE) {
				eMenu = ClearMenu::CLEAR_STAGESELECT;
			}

		}
		/*! �ŏI�X�e�[�W�łȂ� */
		else {
			if (eMenu != ClearMenu::CLEAR_NEXTSTAGE) {
				eMenu = static_cast<ClearMenu>(eMenu >> 1);
			}
			else {
				eMenu = ClearMenu::CLEAR_TITLE;
			}
		}
		sound.GetWave("MOVE")->Play();
	}

	/*! �E */
	if (input.GetRightDown()) {

		/*! �ŏI�X�e�[�W��"NEXT STAGE"�������̂ŕ\�����Ȃ����� */
		if (isLastStage) {

			if (eMenu == ClearMenu::CLEAR_STAGESELECT) {
				eMenu = ClearMenu::CLEAR_TITLE;
			}
			else if (eMenu == ClearMenu::CLEAR_TITLE) {
				eMenu = ClearMenu::CLEAR_STAGESELECT;
			}

		}
		/*! �ŏI�X�e�[�W�łȂ� */
		else {
			if (eMenu != ClearMenu::CLEAR_TITLE) {
				eMenu = static_cast<ClearMenu>(eMenu << 1);
			}
			else {
				eMenu = ClearMenu::CLEAR_NEXTSTAGE;
			}
		}
		sound.GetWave("MOVE")->Play();
	}

	/*! ���� */
	if (input.GetDecideDown()) {
		eMenu = static_cast<ClearMenu>(0b00001000 | eMenu);
	}

	/*****************************************/
	/*! �����肳��ĂȂ���Ώ������X�L�b�v�� */
	/*****************************************/
	const bool isSkip = (0b00001000 & eMenu) == 0;
	if (isSkip) { return; }/*!< ���肳��ĂȂ��̂ŏ����𔲂��� */


	/*****************************************/
	/*!		���ȍ~�͌��肳�ꂽ�u�Ԃ̏�����	 */
	/*****************************************/
	ClearMenu menu = static_cast<ClearMenu>(0b00000111 & eMenu);

	/*! ���艹�Đ� */
	sound.GetWave("ENTER")->Play();
	sound.GetWave("STAGE_CLEAR")->Stop();


	/*! ���̃X�e�[�W�̃f�[�^��ǂݍ���ł��� */
	if (menu == ClearMenu::CLEAR_NEXTSTAGE) {
		int next = StageLoader::GetInstance().GetLoadedStageNo() + 1;
		StageLoader::GetInstance().DataLoad(next);
	}

	/*! }�t�F�[�h�A�E�g���� */
	FadeManager::GetInstance().StartFadeOutOnly(FADE_FRAME);

}