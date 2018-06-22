/*!
	@file	InputManager.h
	@date	2018/04/13
	@author	�ԏ� �G�P
	@brief	���͊Ǘ��N���X
*/
#pragma once
#include "../GamePad.h"

class InputManager
{
public:
	~InputManager();
	static InputManager&GetInstance() {
		static InputManager instance;
		return instance;
	}

	/*! ���� */
	void Initialize();
	void Update();
	void Finalize();
	bool GetLeft();
	bool GetLeftDown();
	bool GetRight();
	bool GetRightDown();
	bool GetStartDown();
	bool GetUpActionDown();
	bool GetDownActionDown();
	bool GetUpDown();
	bool GetUnderDown();
	bool GetDecideDown();
	bool GetBackDown();
private:
	InputManager();

	static const int RESET_FRAME;
	GamePad* pGamePad;

	int8_t isInput;	/*!< �r�b�g�}�X�N��p�����W���C�X�e�B�b�N�̘A�����͖h�~���� */
	/*!
		�}�X�N�ӏ�
		��:0b0001
		��:0b0010
		��:0b0100
		�E:0b1000
	*/
};

