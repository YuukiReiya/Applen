/*!
	@file	InputManager.h
	@date	2018/04/13
	@author	番場 宥輝
	@brief	入力管理クラス
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

	/*! 入力 */
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

	int8_t isInput;	/*!< ビットマスクを用いたジョイスティックの連続入力防止判定 */
	/*!
		マスク箇所
		上:0b0001
		下:0b0010
		左:0b0100
		右:0b1000
	*/
};

