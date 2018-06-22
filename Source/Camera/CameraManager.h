/**
* @file		CameraManager.h
* @brief	カメラ管理クラス
* @author	東　恭亮
* @date　	2018/02/20
*/
#pragma once
#include <DirectXMath.h>

class CameraManager
{
private:
	CameraManager();
	~CameraManager() {};

	DirectX::XMVECTOR xvEyePt;
	DirectX::XMVECTOR xvLookatPt;
	DirectX::XMVECTOR xvUpVec;

	static constexpr float PI = 3.14159265f;

	// カメラ範囲
	static constexpr float FOV_PI = PI / 4.0f;
	static constexpr float FOV_MIN = 0.1f;
	static constexpr float FOV_MAX = 100.0f;

public:
	static CameraManager & GetInstance()
	{
		static CameraManager c;
		return c;
	}

	void CameraSet
	(
		DirectX::XMFLOAT3 eye,
		DirectX::XMFLOAT3 look = { 0.0,0.0f,0.0f },
		DirectX::XMFLOAT3 up = { 0.0,1.0f,0.0f }
	);

	//ワールド変換用
	DirectX::XMMATRIX xmView;
	DirectX::XMMATRIX xmProj;

	DirectX::XMFLOAT3 GetEye()
	{
		DirectX::XMFLOAT3 tmp;
		DirectX::XMStoreFloat3(&tmp, xvEyePt);
		return tmp;
	}

	DirectX::XMFLOAT3 GetLook() 
	{ 
		DirectX::XMFLOAT3 temp;
		DirectX::XMStoreFloat3(&temp,xvLookatPt);
		return temp;
	};
};