#include "BlockBase.h"

/*! 
	@brief	名前空間
*/
using namespace DirectX;

/*! 
	@def	定数宣言
*/

/*! 左上座標 */
const XMFLOAT3 BlockBase::LEFT_TOP_POSITION = {
	-5.25f,	/*!< x */
	3.35f,	/*!< y */
	1		/*!< z */
};

/*! オブジェクト間のオフセット */
const XMFLOAT2 BlockBase::OFFSET = {
	1.82f,	/*!< x */
	1.82f,	/*!< y */
};

/*! 1フレーム当たりの移動量(等速直線運動) */
const float BlockBase::MOVE_RER_FRAME = 0.2f;

/*! IDの桁指定オフセット */
const int BlockBase::ID_OPTION1 = 100000;	/*!< 左から1文字目の桁を指定するオフセット */
const int BlockBase::ID_OPTION2 = 10000;	/*!< 左から2文字目の桁を指定するオフセット */
const int BlockBase::ID_OPTION3 = 1000;		/*!< 左から3文字目の桁を指定するオフセット */
const int BlockBase::ID_OPTION4 = 100;		/*!< 左から4文字目の桁を指定するオフセット */
const int BlockBase::ID_OPTION5 = 10;		/*!< 左から5文字目の桁を指定するオフセット */
const int BlockBase::ID_OPTION6 = 1;		/*!< 左から6文字目の桁を指定するオフセット */

/*!
	@brief	初期化
*/
void BlockBase::Initialize()
{
	from.x = LEFT_TOP_POSITION.x + index.x*OFFSET.x;
	from.y = LEFT_TOP_POSITION.y - index.y*OFFSET.y;
	from.z = LEFT_TOP_POSITION.z;
	to = from;
	eState = MoveState::WAIT;
	actionFinishedFunc = nullptr;
}

/*!
	@brief	破棄
*/
void BlockBase::Finalize()
{
}

/*!
	@brief	上移動開始命令
*/
void BlockBase::StartMoveUp(std::function<void()> func)
{
	actionFinishedFunc = func;
	to = from;
	to.y += OFFSET.y;
	eState = MoveState::UP;
	index.y -= 1;
}

/*!
	@brief	下移動開始命令
*/
void BlockBase::StartMoveDown(std::function<void()> func)
{
	actionFinishedFunc = func;
	to = from;
	to.y -= OFFSET.y;
	eState = MoveState::DOWN;
	index.y += 1;
}

/*!
	@brief	移動
	@detail	下記移動関数ラッピング
*/
void BlockBase::Move()
{
	/*! ステート切り替え */
	switch (eState)
	{
		/*! 移動待機 */
	case BlockBase::WAIT:
		break;
		/*! 上移動 */
	case BlockBase::UP:
		MoveUp();
		break;
		/*! 下移動 */
	case BlockBase::DOWN:
		MoveDown();
		break;
	default:
		assert("Invalid value");
		break;
	}

}

/*!
	@brief	上移動
*/
void BlockBase::MoveUp()
{	
	/*! 移動 */
	if (from.y < to.y) {
		from.y += MOVE_RER_FRAME;
	}
	/*! 移動終了 */
	else {
		from = to;
		eState = MoveState::WAIT;
		/*! コールバック関数の呼び出し */
		if (actionFinishedFunc != nullptr) {
			actionFinishedFunc();			/*!< 実行 */
			actionFinishedFunc = nullptr;	/*!< 初期化 */
		}
	}
}

/*!
	@brief	下移動
*/
void BlockBase::MoveDown()
{
	/*! 移動 */
	if (from.y > to.y) {
		from.y -= MOVE_RER_FRAME;
	}
	/*! 移動終了 */
	else {
		from = to;
		eState = MoveState::WAIT;
		/*! コールバック関数の呼び出し */
		if (actionFinishedFunc != nullptr) {
			actionFinishedFunc();			/*!< 実行 */
			actionFinishedFunc = nullptr;	/*!< 初期化 */
		}
	}
}
