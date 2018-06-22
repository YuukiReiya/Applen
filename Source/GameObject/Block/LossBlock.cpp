#include "LossBlock.h"

/*!
	@brief	名前空間
*/
using namespace std;
using namespace DirectX;

/*!
	@def	定数宣言
*/
const float LossBlock::MAX_ALPHA = 0.6f;		/*!< 描画するオブジェクトのα値の最大値 */
const float LossBlock::MIN_ALPHA = 0.4f;		/*!< 描画するオブジェクトのα値の最小値 */
const int LossBlock::TRANSRATION_FRAME = 150;	/*!< α値が遷移するフレーム */

/*! ブレンドする割合(合計10になるよう設定する) */
const XMINT2 LossBlock::BLEND_RATIO{
	8,	/*!< 表示時間の割合 */
	2,	/*!< 非表示時間の割合 */
};

/*!
	@brief	コンストラクタ
*/
LossBlock::LossBlock():BlockBase(),alpha(MAX_ALPHA)
{
	sign = -1;/*!< 初期はα値の減算から入る */
	count = 0;
}

/*!
	@brief	デストラクタ
*/
LossBlock::~LossBlock()
{
}

/*!
	@brief	初期化
*/
void LossBlock::Initialize()
{
	BlockBase::Initialize();
	/*! 消失回数の設定 */
	lossNum = id / ID_OPTION1;
	/*! 破棄フラグ */
	isDelete = false;
	/*! 符号 */
	sign = -1;/*!< 最初はα値の減算から処理する */
}

/*!
	@brief	更新
*/
void LossBlock::Update()
{
	Move();
	Blend();
}

/*!
	@brief	上移動開始命令
	@detail	コールバック関数に消失回数の減少処理を追加し処理を行わせる
*/
void LossBlock::StartMoveUp(std::function<void()> func)
{
	function<void()> compileFunc;

	/*! 関数のラッピング */
	compileFunc = [=] 
	{
		if (func != nullptr) {
			func();
		}
		--lossNum;
		if (lossNum <= 0) {
			isDelete = true;
		}
	};

	BlockBase::StartMoveUp(compileFunc);
}

/*!
	@brief	下移動開始命令
	@detail	コールバック関数に消失回数の減少処理を追加し処理を行わせる
*/
void LossBlock::StartMoveDown(std::function<void()> func)
{
	function<void()> compileFunc;

	/*! 関数のラッピング */
	compileFunc = [=]
	{
		if (func != nullptr) {
			func();
		}
		--lossNum;
		if (lossNum <= 0) {
			isDelete = true;
		}
	};


	BlockBase::StartMoveDown(compileFunc);
}

/*!
	@brief	α値のブレンド
*/
void LossBlock::Blend()
{
	/*! カウンタの加算 */
	++count;

	/*! 割合 */
	int A = TRANSRATION_FRAME * BLEND_RATIO.x / 10;	/*!< 減算時の遷移フレームの表示時間割合 */
	int B = TRANSRATION_FRAME * BLEND_RATIO.y / 10;	/*!< 減算時の遷移フレームの非表示時間割合 */
	int C = A + B + B;								/*!< 加算時の遷移フレームの表示時間割合 */
	int D = A + C;									/*!< 加算時の遷移フレームの非表示時間割合 */

	float val;/*!< α値の加算量を表す変数 */

	const float difference = MAX_ALPHA - MIN_ALPHA;/*!< MAXとMINの差分 */

	/*!
		α値の高い状態を長く、低い状態を短くしたいのでフレームに応じて
		α値の変異を緩やかな状態/激しい状態の2種で変異を行わせる
	*/

	/*! 緩やかな減算 */
	if (count < A) {
		val = difference / A;
		sign = -1;
	}
	/*! 激しい減算 */
	else if (count < A + B) {
		val = difference / B;
		sign = -1;
	}
	/*! 激しい加算 */
	else if (count < C) {
		val = difference / B;
		sign = 1;
	}
	/*! 緩やかな加算 */
	else if (count < D) {
		val = difference / A;
		sign = 1;
	}
	/*! 1フレーム待機し誤差を消す */
	else {
		val = 0;
	}

	/*! α値の変異 */
	alpha += val * sign;

	/*! カウンタのリセット */
	if (count == D) {
		count = 0;
	}
}
