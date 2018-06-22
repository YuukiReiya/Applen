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

//�O���[�o���ϐ�
MAIN* g_pMain = nullptr;

//�֐��v���g�^�C�v�̐錾
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* �A�v���P�[�V�����̃G���g���[�֐�
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
int APIENTRY _tWinMain(HINSTANCE hInstance,HINSTANCE,LPTSTR,INT)
{
	/*! ���������[�N�ӏ����� */
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//���������[�N�����
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
* �E�B���h�E�v���V�[�W��
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		switch ((char)wParam)
		{
		case VK_ESCAPE://ESC�L�[�ŏC��
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
* �E�B���h�E�쐬
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT MAIN::InitWindow(HINSTANCE hInstance)
 {
	 // �E�B���h�E�̒�`
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

	//�E�B���h�E�̍쐬
	m_hWnd=CreateWindow(APP_NAME,APP_NAME,WS_OVERLAPPEDWINDOW,
		0,0,WINDOW_WIDTH,WINDOW_HEIGHT,0,0,hInstance,0);
	 if(!m_hWnd)
	 {
		 MessageBox(0, L"�E�B���h�E�쐬�Ɏ��s���܂���", 0, MB_OK | MB_ICONWARNING);
		 return E_FAIL;
	 }
	 //�E�C���h�E�̕\��
	 ShowWindow(m_hWnd,SW_SHOW);
	 UpdateWindow(m_hWnd) ;

	 return S_OK;
 }

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* ���b�Z�[�W���[�v
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
 bool MAIN::MessageLoop()
 {
	 /*! �C���v�b�g�}�l�[�W���[ */
	 InputManager::GetInstance().Initialize();

	 /*! �t�F�[�h�}�l�[�W���[ */
	 FadeManager::GetInstance().Initialize();

	 /*! ���f���ꊇ���[�h */
	 ModelManager::GetInstance().InitializeModel();

	 /*! �T�E���h�}�l�[�W���[ */
	 SoundManager::GetInstance().Init();

	 /*! �X�e�[�W�ǂݍ��݃}�l�[�W���[ */
	 StageLoader::GetInstance().Initialize();

	 /*! �T�E���h���X�g */
	 SoundStorage::GetInstance().Initialize();

	 //�V�[���}�l�[�W��
	 pManager = new SceneManager;
	 pManager->Init();

	 // message�i�[�\����
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
			 //			 �C���v�b�g			 //
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
 * �X�V
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
 * �`��
 */
 ///////////////////////////////////////////////////////////////////////////////////////////////////
 bool MAIN::Render()
 {
	 pManager->Render();

	 //��ʍX�V�i�o�b�N�o�b�t�@���t�����g�o�b�t�@�Ɂj
	 DeviceManager::GetInstance().pSwapChain->Present(0, 0);

	 return true;
 }

 //*************************************************************
 // @brief Fps
 // @note Fps�̒���
 //*************************************************************
 void MAIN::Fps()
 {
	 //FPS����
	 while (timeGetTime() - start < 1000 / FPS_RATE)
	 {
		 Sleep(1);
	 }
	 start = timeGetTime();
 }

 //*************************************************************
 // @brief Fps
 // @note Fps�̕\��
 //*************************************************************
 void MAIN::FpsRate()
 {
	 //���������[�N
	 _CrtDumpMemoryLeaks();

	 //FPS�v�Z�\��
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