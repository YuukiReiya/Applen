/**
	* @file		MAIN.h
	* @brief	�E�B���h�E��������R���g���[���܂�
	* @author	���@����
	* @date�@	2018/02/20
*/
#pragma once
#include "Device/DeviceManager.h"
#include "Scene/Manager/SceneManager.h"

class MAIN
{
private:
	SceneBase*	  pScene;
	SceneManager* pManager;

	long start;				//fps�v���p
	static constexpr const uint32_t FPS_RATE = 60;

	bool Render();
	bool Update();

public:
	MAIN() {};
	~MAIN() {};

	HWND m_hWnd;

	HRESULT InitWindow(HINSTANCE hInstance);
	bool MessageLoop();
	void Fps();
	void FpsRate();
};