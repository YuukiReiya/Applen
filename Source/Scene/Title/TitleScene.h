/**
	* @file		TitleScene.h
	* @brief	�^�C�g���V�[��
	* @author	�ԏ� �G�P
	* @date�@	2018/02/20
*/
#pragma once
#include "../Manager/SceneManager.h"
#include "../../Camera/CameraManager.h"
#include "../../BackGround/TitleBackGround.h"
#include "../../Sprite/SpriteBase.h"
#include "../../Sprite/Sprite.h"


class TitleScene final
	: public Scene
{
public:
	TitleScene();
	~TitleScene() {};

	void Init();
	SceneBase* Update(SceneManager* root);
	void Render();
	void Destroy();
private:

	/*! �萔 */
	static const int FADE_FRAME;

	/*! �^�C�g�����j���[ */
	enum TitleMenu :int8_t {
		/*!
		��3�������j���[�̑I����4���ڂ͌���ɔ���
		ex)
		00000100:"EXIT"�I�����
		��
		00001100:"EXIT"������
		*/
		TITLE_PLAY = 0b0001,
		TITLE_CREDIT = 0b0010,
		TITLE_EXIT = 0b0100,
	}eMenu;

	/*! ���L�X�e�[�g�̊�� */
	struct StateBase {
		StateBase() {};
		StateBase(TitleScene* scene) :next(nullptr) { this->scene = scene; }
		virtual void Initialize() {};
		virtual void Finalize()		= 0;
		virtual void Update()		= 0;
		virtual void Render()		= 0;
		SceneBase* next;
	protected:
		TitleScene * scene;
	};

	/*! "PLAY"�I���X�e�[�g */
	struct PlayState
		:public StateBase
	{
		PlayState(TitleScene* scene) :StateBase(scene) {}
		void Initialize();
		void Finalize();
		void Update();
		void Render();
	};

	/*! "CREDIT"�I���X�e�[�g */
	struct CreditState
		:public StateBase
	{
		CreditState(TitleScene* scene) :StateBase(scene) {}
		void Initialize();
		void Finalize();
		void Update();
		void Render();
	private:
		/*! �萔 */
		static const std::string HASH_NAME;				/*!< �\������e�N�X�`���̃n�b�V���� */
		static const int SCALE_FRAME;					/*!< �g��k���ɂ�����t���[�� */
		static const DirectX::XMFLOAT2 POSITION_OFFSET;	/*!< �X�P�[���ω��ɔ����ω�������W�̃I�t�Z�b�g */
		static const DirectX::XMFLOAT2 TEXTURE_SCALE;	/*!< �e�N�X�`���̃X�P�[�� */
		
		/*! �ϐ� */
		int count;
		bool isOpen;

		/*! �֐� */
		void Scaling();
		void Close();
	};

	/*! "EXIT"�I���X�e�[�g */
	struct ExitState
		:public StateBase
	{
		ExitState(TitleScene* scene) :StateBase(scene) {}
		void Initialize();
		void Finalize();
		void Update();
		void Render();
	};


	/*! �B���֐� */
	void Input();

	/*! �����o�ϐ� */
	StateBase* pState;			/*!< �X�e�[�g�p�ϐ� */
	umap_str_sprite pSprites;	/*!< �\���e�N�X�`�� */
	TitleBackGround* pTbg;

};