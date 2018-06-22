#include "CameraManager.h"
#include "../Common.h"

using namespace DirectX;

CameraManager::CameraManager()
{
	// �s��̏�����
	xmView = XMMatrixIdentity();
	xmProj = XMMatrixIdentity();

	xvEyePt	  = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	xvLookatPt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	xvUpVec    = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* Camera�̏�����
* @param (eye) ���_
* @param (look) �����_
* @param (up) ����ʒu
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

	// ������W�̃r���[���쐬
	xmView = XMMatrixLookAtLH(xvEyePt, xvLookatPt, xvUpVec);

	// �v���W�F�N�V�����g�����X�t�H�[���i�ˉe�ϊ��j
	xmProj = XMMatrixPerspectiveFovLH(FOV_PI, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, FOV_MIN, FOV_MAX);
}