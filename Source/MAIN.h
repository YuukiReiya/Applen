/**
	* @file		MAIN.h
	* @brief	ウィンドウ生成からコントロールまで
	* @author	東　恭亮
	* @date　	2018/02/20
*/
#pragma once
#include "Device/DeviceManager.h"
#include "Scene/Manager/SceneManager.h"

class MAIN
{
private:
	SceneBase*	  pScene;
	SceneManager* pManager;

	long start;				//fps計測用
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