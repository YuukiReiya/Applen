#include "ResonanceBlock.h"
#include "../../Camera/CameraManager.h"

/*!
	@brief	���O���
*/
using namespace DirectX;

/*!
	@def	�萔�錾
*/

const float ResonanceBlock::PI = 3.14159265f;/*!< �� */
/*! 1�t���[��������̉�]�� */
const XMFLOAT3 ResonanceBlock::ROTATION_PER_FRAME = {
	0.0f,			/*!< x */
	0.02f,			/*!< y */
	0.0f,			/*!< z */
};

/*!
	@brief	�R���X�g���N�^
*/
ResonanceBlock::ResonanceBlock() : BlockBase(), rot({ 0,0,0 })
{
}

/*!
	@brief	�f�X�g���N�^
*/
ResonanceBlock::~ResonanceBlock()
{
}

/*!
	@brief	�X�V
*/
void ResonanceBlock::Update()
{
	Move();
	Rotation();
}

/*!
	@brief	��]
*/
void ResonanceBlock::Rotation()
{
	/*! x */
	if (rot.x<=PI) {
		rot.x += ROTATION_PER_FRAME.x;
	}
	else {
		rot.x = 0;
	}
	/*! y */
	if (rot.y <= PI) {
		rot.y += ROTATION_PER_FRAME.y;
	}
	else {
		rot.y = 0;
	}
	/*! z */
	if (rot.z <= PI) {
		rot.z += ROTATION_PER_FRAME.z;
	}
	else {
		rot.z = 0;
	}
}
