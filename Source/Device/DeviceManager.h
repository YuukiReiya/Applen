/**
	* @file		DeviceManager.h
	* @brief	D3D11COM�Ǘ��N���X
	* @author	���@����
	* @date�@	2018/02/20
*/
#pragma once
#include "../Common.h"
//#include "../Input/GamePad.h"
//#include <dxgidebug.h>
//#include <d3d11sdklayers.h>

//ComPtr�ŊǗ����Ȃ�COM�����[�X�p
template <typename COM>
static inline void SAFE_RELEASE(COM* &p)
{
	if (p != nullptr)
	{
		p->Release();
		p = nullptr;
	}
}

// �ʏ�̃|�C���^�p
template <typename P>
static inline void SAFE_DELETE(P* &p)
{
	typedef char type_must_be_complete[sizeof(P) ? 1 : -1];
	(void)sizeof(type_must_be_complete);

	delete p;
	p = nullptr;
}

// �|�C���^�z��p
template <typename DA>
static inline DA SAFE_DELETE_ARRAY(DA p)
{
	if (p)
	{
		delete[]p;
		p = nullptr;
	}
	return p;
}

// �R���e�i�p
template <typename VI>
static inline void SAFE_DELETE_CONTAINER(VI &p)
{
	VI temp;

	//�����܂Ńf�X�g���N�^���ĂԂ����Ȃ̂�
	//�f�X�g���N�^�ŕK�������������������
	p.swap(temp);
}

//*************************************************************
//�@�N���X
//*************************************************************
class DeviceManager
{
private:
	DeviceManager();
	~DeviceManager();

public:
	HWND hWnd;

	static DeviceManager & GetInstance()
	{
		static DeviceManager dm;
		return dm;
	}

	//Device
	Microsoft::WRL::ComPtr<ID3D11Device>			pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		pDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			pSwapChain;

	//�ʏ��RTV
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	pRenderDSV;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			pRenderDST;
	
	//�[�x�}�b�v
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	pDepthMapTexRTV;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			pDepthMapTex;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			pDepthMapDSTex;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	pDepthMapDSTexDSV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>pDepthMapTexSRV;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	pRasterizerState;

	//�f�o�b�O���[�h
	bool isDebug;

	HRESULT InitD3D11(const HWND m_hWnd);
	void SetNormalRender();
};