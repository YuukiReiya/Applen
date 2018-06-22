/*!
	@file	MainScene.h
	@date	2018/04/04
	@author	�ԏ� �G�P
	@brief	�Q�[���̃��C���V�[��
*/
#pragma once
#include "../Scene.h"
#include "../../Camera/CameraManager.h"
#include "../../GameObject/Player/Player.h"
#include "../../GameObject/StageManager/StageManager.h"
#include "../../GameObject/Squares/SquaresManager.h"
#include "../../Sprite/SpriteBase.h"
#include "../../Sprite/Sprite.h"
#include "../../BackGround/BackGround.h"


class MainScene final:
	public Scene
{
public:
	MainScene();
	~MainScene();


	void Init()override;
	SceneBase* Update(SceneManager* root)override;
	void Render()override;
	void Destroy()override;
private:

	/*! ���L�X�e�[�g�̊�� */
	struct StateBase
	{
		StateBase(MainScene*main) :next(nullptr) { this->main = main; }
		virtual void Initialize()	= 0;
		virtual void Finalize()		= 0;
		virtual StateBase* Update()	= 0;
		virtual void Render()		= 0;
		SceneBase* next;
	protected:
		MainScene* main;
	};

	/*! �Q�[�����̃X�e�[�g */
	struct GameState :
		public StateBase
	{
		GameState(MainScene*main) :StateBase(main) {}
		void Initialize()override;
		void Finalize()override;
		StateBase* Update()override;
		void Render()override;
	};

	/*! �|�[�Y���̃X�e�[�g */
	struct PauseState :
		public StateBase
	{
		PauseState(MainScene* main) :StateBase(main) {}
		void Initialize()override;
		void Finalize()override;
		StateBase* Update()override;
		void Render()override;
		/*! �|�[�Y���j���[ */
		enum PauseMenu:int8_t {
			/*!
				��4�������j���[�̑I����5���ڂ͌���ɔ���
				ex)
					00001000:�^�C�g���I�����
							��
					00011000:�^�C�g��������
			*/
			PAUSE_PLAY			= 0b0001,	/*!< �Q�[���ĊJ */
			PAUSE_RETRY			= 0b0010,	/*!< �X�e�[�W�����g���C */
			PAUSE_STAGESELECT	= 0b0100,	/*!< �X�e�[�W�Z���N�g�֖߂� */
			PAUSE_TITLE			= 0b1000,	/*!< �^�C�g���֖߂� */
		}eMenu;

	private:
		void Input();
	};

	/*! �N���A���̃X�e�[�g */
	struct ClearState:
		public StateBase
	{
		ClearState(MainScene* main) :StateBase(main) {}
		void Initialize()override;
		void Finalize()override;
		StateBase* Update()override;
		void Render()override;
		/*! �N���A���j���[ */
		enum ClearMenu :int8_t {
			/*!
			��3�������j���[�̑I����4���ڂ͌���ɔ���
			ex)
			00000100:�^�C�g���I�����
			��
			00001100:�^�C�g��������
			*/
			CLEAR_NEXTSTAGE		= 0b0001,	/*!< ���̃X�e�[�W */
			CLEAR_STAGESELECT	= 0b0010,	/*!< �X�e�[�W�Z���N�g */
			CLEAR_TITLE			= 0b0100,	/*!< �^�C�g�� */
		}eMenu;
	private:
		static const int WAIT_FRAME;
		int count;
		void Input();
		bool isLastStage;
	};

	/*! �萔 */
	static const int FADE_FRAME;

	/*! �X�e�[�g */
	StateBase* pState;

	/*! �}�l�[�W���[�N���X */
	SquaresManager*		pSquareManager;
	StageManager*	pStageManager;
	Player*			pPlayer;

	/*! �X�v���C�g */
	umap_str_sprite pSprites;

	/*! �w�i */
	BackGround* pBack;

	/*! �B���֐� */
	void StartBGM();
	void StopBGM();
};

