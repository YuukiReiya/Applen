/**
	* @file		Common.h
	* @brief	プロジェクト設定
	* @author	東　恭亮
	* @date　	2018/02/20
*/
#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <d3dCompiler.h>
#include <stdint.h>
#include <DirectXMath.h>
#include <wrl.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")
#pragma comment(lib,"winmm.lib")

// プロジェクト設定
static constexpr uint32_t WINDOW_WIDTH	= 1920; 
static constexpr uint32_t WINDOW_HEIGHT = 1080;
static constexpr LPCWSTR APP_NAME = L"Appllen";

#ifdef _DEBUG
#include <crtdbg.h>
#define	new	new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif