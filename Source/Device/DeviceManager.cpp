#include "DeviceManager.h"

using namespace DirectX;

DeviceManager::DeviceManager()
{
	isDebug = false;

#ifdef _DEBUG
	isDebug = true;
#endif 
}

DeviceManager::~DeviceManager()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
	* @fn
	* D3D初期化
	* @param (hwnd)	出力先のウィンドウ
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT DeviceManager::InitD3D11(const HWND hwnd)
{
	hWnd = hwnd;

	// デバイスとスワップチェーンの作成
	DXGI_SWAP_CHAIN_DESC sd;
	SecureZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = WINDOW_WIDTH;
	sd.BufferDesc.Height = WINDOW_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.Windowed = true;

	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = nullptr;

	int isDevice = 0;

	//大半がComptrで、クラス自体がSingletonなため不必要
//#ifdef _DEBUG
//	isDevice = D3D11_CREATE_DEVICE_DEBUG;
//#endif

	if (FAILED(D3D11CreateDeviceAndSwapChain(
		0, 
		D3D_DRIVER_TYPE_HARDWARE, 0,
		isDevice,
		&pFeatureLevels, 
		1,
		D3D11_SDK_VERSION,
		&sd, pSwapChain.GetAddressOf(),
		pDevice.GetAddressOf(),
		pFeatureLevel,
		pDeviceContext.GetAddressOf()))
		)
	{
		MessageBox(0, L"SwapChainの作成に失敗しました", 0, MB_OK | MB_ICONWARNING);
		return E_FAIL;
	}

	//レンダーターゲットビューの作成
	ID3D11Texture2D *pBackBuffer;
	if(FAILED(pSwapChain->GetBuffer(0,__uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
	{
		MessageBox(0, L"バッファの確保に失敗しました", 0, MB_OK | MB_ICONWARNING);
		return E_FAIL;
	}
	if (FAILED(pDevice->CreateRenderTargetView(pBackBuffer, 0, pRenderTargetView.GetAddressOf())))
	{
		MessageBox(0, L"RenderTargetViewの作成に失敗しました", 0, MB_OK | MB_ICONWARNING);
		return E_FAIL;
	}
	SAFE_RELEASE(pBackBuffer);

	//深度ステンシルビューとテクスチャを作成
	D3D11_TEXTURE2D_DESC texDesc;
	SecureZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = WINDOW_WIDTH;
	texDesc.Height = WINDOW_HEIGHT;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_D32_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	if (FAILED(pDevice->CreateTexture2D(&texDesc, 0, pRenderDST.GetAddressOf())))
	{
		MessageBox(0, L"深度テクスチャの作成に失敗しました", 0, MB_OK | MB_ICONWARNING);
		return E_FAIL;
	}
	if (FAILED(pDevice->CreateDepthStencilView(pRenderDST.Get(), 0, pRenderDSV.GetAddressOf())))
	{
		MessageBox(0, L"深度ステンシルビューの作成に失敗しました", 0, MB_OK | MB_ICONWARNING);
		return E_FAIL;
	}

	//レンダーターゲットビューと深度ステンシルビューをパイプラインにバインド
	//pDeviceContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pRenderDSV.Get());
	pDeviceContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pRenderDSV.Get());

	//キューブ深度マップテクスチャの作成
	texDesc.Width = 512;
	texDesc.Height = 512;
	texDesc.Format = DXGI_FORMAT_R32_FLOAT;
	texDesc.ArraySize = 6;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET 
		| D3D11_BIND_SHADER_RESOURCE;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS 
		| D3D11_RESOURCE_MISC_TEXTURECUBE;
	if (FAILED(pDevice->CreateTexture2D(&texDesc, 0, pDepthMapTex.GetAddressOf())))
	{
		MessageBox(0, L"深度マップテクスチャの作成に失敗しました", 0, MB_OK | MB_ICONWARNING);
		return E_FAIL;
	}

	//深度マップテクスチャ用レンダーターゲットビューの作成
	D3D11_RENDER_TARGET_VIEW_DESC mapRT;
	SecureZeroMemory(&mapRT, sizeof(mapRT));
	mapRT.Format = texDesc.Format;
	mapRT.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	mapRT.Texture2DArray.ArraySize = 6;
	if (FAILED(pDevice->CreateRenderTargetView(pDepthMapTex.Get(),&mapRT, pDepthMapTexRTV.GetAddressOf())))
	{
		MessageBox(0, L"深度マップテクスチャ用RTVの作成に失敗しました", 0, MB_OK | MB_ICONWARNING);
		return E_FAIL;
	}

	//深度マップテクスチャをRTにする際のDSV用のテクスチャ作成
	texDesc.Format = DXGI_FORMAT_D32_FLOAT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	if(FAILED(pDevice->CreateTexture2D(&texDesc, 0, pDepthMapDSTex.GetAddressOf())))
	{
		MessageBox(0, L"深度マップテクスチャ用DSVテクスチャの作成に失敗しました", 0, MB_OK | MB_ICONWARNING);
		return E_FAIL;
	}

	//上のテクスチャに対してのDSVを作成
	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc;
	SecureZeroMemory(&DSVDesc, sizeof(DSVDesc));
	DSVDesc.Format = DXGI_FORMAT_D32_FLOAT;
	DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	DSVDesc.Texture2DArray.ArraySize = 6;
	DSVDesc.Flags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	if(FAILED(pDevice->CreateDepthStencilView(pDepthMapDSTex.Get(), 0,pDepthMapDSTexDSV.GetAddressOf())))
	{
		MessageBox(0, L"深度マップテクスチャ用DSVテクスチャのDSV作成に失敗しました", 0, MB_OK | MB_ICONWARNING);
		return E_FAIL;
	}

	//深度マップテクスチャ用シェーダーリソースビューの作成
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	SecureZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SRVDesc.Texture2D.MipLevels = 1;
	if (FAILED(pDevice->CreateShaderResourceView(pDepthMapTex.Get(),&SRVDesc,pDepthMapTexSRV.GetAddressOf())))
	{
		MessageBox(0, L"深度マップテクスチャ用のSRVの作成に失敗しました", 0, MB_OK | MB_ICONWARNING);
		return E_FAIL;
	}

	//ビューポートの設定
	D3D11_VIEWPORT vp;
	SecureZeroMemory(&vp, sizeof(vp));
	vp.Width = WINDOW_WIDTH;
	vp.Height = WINDOW_HEIGHT;
	vp.MaxDepth = 1.0f;
	pDeviceContext->RSSetViewports(1, &vp);

	//ラスタライズ設定
	D3D11_RASTERIZER_DESC rdc;
	SecureZeroMemory(&rdc, sizeof(rdc));
	rdc.CullMode = D3D11_CULL_NONE;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = 1;

	if (FAILED(pDevice->CreateRasterizerState(&rdc, pRasterizerState.GetAddressOf())))
	{
		MessageBox(0, L"深度ステンシルビューの作成に失敗しました", 0, MB_OK | MB_ICONWARNING);
		return E_FAIL;
	}
	pDeviceContext->RSSetState(pRasterizerState.Get());

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* 通常描画
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void DeviceManager::SetNormalRender()
{
	// プロジェクショントランスフォーム
	//DeviceManager::GetInstance().maProj = XMMatrixPerspectiveFovLH(D3DX_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 50.0f);

	float ClearColor[4] = { 0,0,0,1 };

	//レンダーターゲット
	pDeviceContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(),pRenderDSV.Get());

	//画面クリア
	pDeviceContext->ClearRenderTargetView(pRenderTargetView.Get(), ClearColor);

	//深度バッファクリア
	pDeviceContext->ClearDepthStencilView(pRenderDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//深度テクスチャをシェーダに渡す
	pDeviceContext->PSSetShaderResources(1,1,pDepthMapTexSRV.GetAddressOf());
}