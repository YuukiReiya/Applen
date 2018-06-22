/**
* @file		SpriteBase.h
* @brief	Spriteに必要なデータの初期化と描画
* @author	東　恭亮
* @date　	2018/02/20
*/
#pragma once
#include "../Device/DeviceManager.h"

//*************************************************************
//　各種構造体
//*************************************************************
struct SPRITE_CONSTANT_BUFFER
{
	DirectX::XMMATRIX xmWVP;
	DirectX::XMFLOAT4 xfColor;
	DirectX::XMFLOAT4 xfAlpha;
};

struct SimpleVertex
{
	DirectX::XMFLOAT3 xfPos;
	DirectX::XMFLOAT2 xfTex;
};

struct TEXTURE_DATA
{
	char filePath[100];
	DirectX::XMFLOAT3 xfPos;
	DirectX::XMFLOAT3 xfRot;
	DirectX::XMFLOAT3 xfScale;
	DirectX::XMFLOAT2 xfSize;
	DirectX::XMFLOAT4 xfColor;
	bool isFollow;
};

//*************************************************************
//　クラス
//*************************************************************
class SpriteBase
{
private:
	DirectX::XMMATRIX MatrixConvert();
	DirectX::XMMATRIX CameraFollow();

	Microsoft::WRL::ComPtr<ID3D11InputLayout>	pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>	pSamplerState;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11BlendState>	pBlendState;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTexture;
	ID3D11Buffer* pConstantBuffer;
	TEXTURE_DATA* texData;

	//画像scaleが1.0fを想定した場合の比率
	static constexpr float TEX_SCALE_X = 100.0f;
	static constexpr float TEX_SCALE_Y = 100.0f;
	static constexpr float TEX_SCALE_Z = 1.0f;

public:
	SpriteBase();
	~SpriteBase();

	HRESULT Init(TEXTURE_DATA* td);
	void Render();
	void Update(TEXTURE_DATA* td);
	TEXTURE_DATA* GetTexData() { return texData; }
	void SetScale(DirectX::XMFLOAT2 scale) { texData->xfScale = { scale.x,scale.y,TEX_SCALE_Z }; }
	void SetPos(DirectX::XMFLOAT2 pos) { texData->xfPos = { pos.x,pos.y , texData->xfPos.z }; }
	DirectX::XMFLOAT2 GetPos2D()const { return { texData->xfPos.x,texData->xfPos.y }; }
	DirectX::XMFLOAT2 GetScale()const { return { texData->xfScale.x,texData->xfScale.y }; }
};