/**
	* @file		DeviceManager.h
	* @brief	D3D11COM管理クラス
	* @author	東　恭亮
	* @date　	2018/02/20
*/
#pragma once
#include "../Common.h"
//#include "../Input/GamePad.h"
//#include <dxgidebug.h>
//#include <d3d11sdklayers.h>

//ComPtrで管理しないCOMリリース用
template <typename COM>
static inline void SAFE_RELEASE(COM* &p)
{
	if (p != nullptr)
	{
		p->Release();
		p = nullptr;
	}
}

// 通常のポインタ用
template <typename P>
static inline void SAFE_DELETE(P* &p)
{
	typedef char type_must_be_complete[sizeof(P) ? 1 : -1];
	(void)sizeof(type_must_be_complete);

	delete p;
	p = nullptr;
}

// ポインタ配列用
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

// コンテナ用
template <typename VI>
static inline void SAFE_DELETE_CONTAINER(VI &p)
{
	VI temp;

	//あくまでデストラクタを呼ぶだけなので
	//デストラクタで必ず解放処理を書くこと
	p.swap(temp);
}

//*************************************************************
//　クラス
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

	//通常のRTV
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	pRenderDSV;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			pRenderDST;
	
	//深度マップ
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	pDepthMapTexRTV;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			pDepthMapTex;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			pDepthMapDSTex;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	pDepthMapDSTexDSV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>pDepthMapTexSRV;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	pRasterizerState;

	//デバッグモード
	bool isDebug;

	HRESULT InitD3D11(const HWND m_hWnd);
	void SetNormalRender();
};