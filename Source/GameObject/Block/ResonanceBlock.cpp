#include "ResonanceBlock.h"
#include "../../Camera/CameraManager.h"

/*!
	@brief	名前空間
*/
using namespace DirectX;

/*!
	@def	定数宣言
*/

const float ResonanceBlock::PI = 3.14159265f;/*!< π */
/*! 1フレームあたりの回転量 */
const XMFLOAT3 ResonanceBlock::ROTATION_PER_FRAME = {
	0.0f,			/*!< x */
	0.02f,			/*!< y */
	0.0f,			/*!< z */
};

/*!
	@brief	コンストラクタ
*/
ResonanceBlock::ResonanceBlock() : BlockBase(), rot({ 0,0,0 })
{
}

/*!
	@brief	デストラクタ
*/
ResonanceBlock::~ResonanceBlock()
{
}

/*!
	@brief	更新
*/
void ResonanceBlock::Update()
{
	Move();
	Rotation();
}

/*!
	@brief	回転
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
