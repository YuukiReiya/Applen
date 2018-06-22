#include "MAIN.h"
#include <stdio.h>
#include <tchar.h>
#include "resource.h"
#include "Input\InputManager\InputManager.h"
#include "Fade\FadeManager.h"
#include "Fbx/Manager/ModelManager.h"
#include "Sound\SoundManager\SoundManager.h"
#include "Sound\Storage\SoundStorage.h"
#include "StageLoader\StageLoader.h"

//グローバル変数
MAIN* g_pMain = nullptr;

//関数プロトタイプの宣言
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* アプリケーションのエントリー関数
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
int APIENTRY _tWinMain(HINSTANCE hInstance,HINSTANCE,LPTSTR,INT)
{
	/*! メモリリーク箇所特定 */
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//メモリリークを特定
#endif

	g_pMain = new MAIN;

	if(g_pMain != nullptr)
	{
		if(SUCCEEDED(g_pMain->InitWindow(hInstance)))
		{
			if(SUCCEEDED(DeviceManager::GetInstance().InitD3D11(g_pMain->m_hWnd)))
			{
				g_pMain->MessageLoop();
			}
		}
		
		SAFE_DELETE(g_pMain);
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* ウィンドウプロシージャ
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		switch ((char)wParam)
		{
		case VK_ESCAPE://ESCキーで修了
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* ウィンドウ作成
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT MAIN::InitWindow(HINSTANCE hInstance)
 {
	 // ウィンドウの定義
	WNDCLASSEX  wc;
	SecureZeroMemory(&wc,sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = APP_NAME;
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	RegisterClassEx(&wc);

	//ウィンドウの作成
	m_hWnd=CreateWindow(APP_NAME,APP_NAME,WS_OVERLAPPEDWINDOW,
		0,0,WINDOW_WIDTH,WINDOW_HEIGHT,0,0,hInstance,0);
	 if(!m_hWnd)
	 {
		 MessageBox(0, L"ウィンドウ作成に失敗しました", 0, MB_OK | MB_ICONWARNING);
		 return E_FAIL;
	 }
	 //ウインドウの表示
	 ShowWindow(m_hWnd,SW_SHOW);
	 UpdateWindow(m_hWnd) ;

	 return S_OK;
 }

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* メッセージループ
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
 bool MAIN::MessageLoop()
 {
	 /*! インプットマネージャー */
	 InputManager::GetInstance().Initialize();

	 /*! フェードマネージャー */
	 FadeManager::GetInstance().Initialize();

	 /*! モデル一括ロード */
	 ModelManager::GetInstance().InitializeModel();

	 /*! サウンドマネージャー */
	 SoundManager::GetInstance().Init();

	 /*! ステージ読み込みマネージャー */
	 StageLoader::GetInstance().Initialize();

	 /*! サウンドリスト */
	 SoundStorage::GetInstance().Initialize();

	 //シーンマネージャ
	 pManager = new SceneManager;
	 pManager->Init();

	 // message格納構造体
	 MSG msg;
	 SecureZeroMemory(&msg, sizeof(msg));

	 while (true)
	 {
		 if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		 {
			 if (msg.message == WM_QUIT) break;

			 TranslateMessage(&msg);
			 DispatchMessage(&msg);
		 }
		 else
		 {
			 //******************************//
			 //			 インプット			 //
			 //******************************//
			 InputManager::GetInstance().Update();

			 g_pMain->Update();
			 g_pMain->Render();
			 g_pMain->Fps();
			 //g_pMain->FpsRate();
		 }
	 }

	 SAFE_DELETE(pManager);

	 return true;
 }

 ///////////////////////////////////////////////////////////////////////////////////////////////////
 /**
 * @fn
 * 更新
 */
 ///////////////////////////////////////////////////////////////////////////////////////////////////
 bool MAIN::Update()
 {
	 pScene = nullptr;
	 pScene = pManager->Update(pManager);

	 return true;
 }

 ///////////////////////////////////////////////////////////////////////////////////////////////////
 /**
 * @fn
 * 描画
 */
 ///////////////////////////////////////////////////////////////////////////////////////////////////
 bool MAIN::Render()
 {
	 pManager->Render();

	 //画面更新（バックバッファをフロントバッファに）
	 DeviceManager::GetInstance().pSwapChain->Present(0, 0);

	 return true;
 }

 //*************************************************************
 // @brief Fps
 // @note Fpsの調整
 //*************************************************************
 void MAIN::Fps()
 {
	 //FPS調整
	 while (timeGetTime() - start < 1000 / FPS_RATE)
	 {
		 Sleep(1);
	 }
	 start = timeGetTime();
 }

 //*************************************************************
 // @brief Fps
 // @note Fpsの表示
 //*************************************************************
 void MAIN::FpsRate()
 {
	 //メモリリーク
	 _CrtDumpMemoryLeaks();

	 //FPS計算表示
	 static DWORD time = 0;
	 static int frame = 0;
	 frame++;

	 char str[50];
	 sprintf_s(str, "fps=%d", frame);
	 if (timeGetTime() - time > 1000)
	 {
		 time = timeGetTime();
		 frame = 0;
		 SetWindowTextA(m_hWnd, str);
	 }
 }