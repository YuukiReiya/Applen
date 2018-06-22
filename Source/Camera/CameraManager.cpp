#include "CameraManager.h"
#include "../Common.h"

using namespace DirectX;

CameraManager::CameraManager()
{
	// 行列の初期化
	xmView = XMMatrixIdentity();
	xmProj = XMMatrixIdentity();

	xvEyePt	  = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	xvLookatPt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	xvUpVec    = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* Cameraの初期化
* @param (eye) 視点
* @param (look) 注視点
* @param (up) 上方位置
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void CameraManager::CameraSet(
	XMFLOAT3 eye,
	XMFLOAT3 look,
	XMFLOAT3 up)
{
	xvEyePt		= XMVectorSet(eye.x,eye.y,eye.z,0.0f);
	xvLookatPt	= XMVectorSet(look.x, look.y, look.z, 0.0f);
	xvUpVec		= XMVectorSet(up.x, up.y, up.z, 0.0f);

	// 左手座標のビューを作成
	xmView = XMMatrixLookAtLH(xvEyePt, xvLookatPt, xvUpVec);

	// プロジェクショントランスフォーム（射影変換）
	xmProj = XMMatrixPerspectiveFovLH(FOV_PI, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, FOV_MIN, FOV_MAX);
}