#include <string>
#include <algorithm>
#include "Player.h"
#include "../../StageLoader/StageLoader.h"
#include "../Squares/SquaresManager.h"
#include "../../Input/InputManager/InputManager.h"

/*!
	@brief	マクロ解除
	@detail	アルゴリズムヘッダのmax/minを使いたい
*/
#undef max
#undef min

/*!
	@brief	名前空間
*/
using namespace std;
using namespace DirectX;

/*!
	@def	定数宣言
*/

/*! 左上座標 */
const XMFLOAT3 Player::LEFT_TOP_OFFSET= {
	-2.4f,	/*!< x */
	1.6f,	/*!< y */
	0.5f	/*!< z */
};

/*! モデルの回転値 */
const XMFLOAT3 Player::MODEL_ROTATION = { 
	-1.6f,	/*!< yaw */
	0.0f,	/*!< pith */
	0.0f	/*!< roll */
};

/*! 1マスあたりの幅 */
//XMFLOAT2 Player::OFFSET= { 
//	0.85f,
//	0.85f 
//};
const float Player::MOVE_MAX_SPEED = 0.1f;		/*!< 移動速度上限 */
const float Player::MOVE_PER_FRAME = 0.05f;		/*!< 1フレーム辺りのベース移動量 */
const float Player::SPEED_ADD_VALUE = 0.02f;	/*!< 移動時に毎フレーム (1+(this))^2 の値がMOVE_PER_FRAMEに乗算される */
const int Player::WAIT_FRAME = 90;				/*!< アニメーションの待機フレーム(このフレーム待機してからアクションする) */

/*!
	@brief	コンストラクタ
*/
Player::Player()
{
	pModel = nullptr;
	pRefInfo = nullptr;
	delayFunc = nullptr;

	OFFSET = {
		0.85f,
		0.85f
	};
}

/*!
	@brief	デストラクタ
*/
Player::~Player()
{
}

/*!
	@brief	初期化
*/
void Player::Initialize()
{
	/*! インスタンス生成 */
	{
		pModel = new PlayerModel();
	}

	/*! 初期化 */
	{
		/*! モデル */
		pModel->Init();
		pModel->SetMotionNum(2);

		/*! パラメータ */
		speed = 0;
		additionValueBase = 0;	/*!< 基礎加算値 */
		eMoveState = MoveState::WAIT;
		isInput = false;

		if (StageLoader::GetInstance().GetLoadedStageNo() >= 8)
		{
			OFFSET = { 0.87f,0.87f };

			if (StageLoader::GetInstance().GetLoadedStageNo() >= 12)
			{
				OFFSET = { 0.88f,0.88f };
			}
		}
	}

}

/*!
	@brief	解放
*/
void Player::Finalize()
{
}

/*!
	@brief	更新
*/
#include "../../Input/Keyboard/Keyboard.h"
void Player::Update()
{
	/*! 更新処理 */
	Move();
	Input();
	Wait();

	/*! 回転更新 */
	rot = MODEL_ROTATION;
	rot.x *= dir;
	pModel->SetRot(rot.x,rot.y,rot.z);

	/*! 座標系更新 */
	pModel->SetPos(pos.x,pos.y,pos.z);
}

/*!
	@brief	描画
*/
void Player::Render()
{
	pModel->Render();
}

/*!
	@brief	ロードデータのセット
*/
void Player::SetLoadData()
{
	/*! 読み込んだデータを設定 */
	auto& load = StageLoader::GetInstance();
	index = load.GetPlayerInfo().index;
	dir = load.GetPlayerInfo().dir;

	/*! 座標変換 */
	Convert(&pos, index);
	/*! 回転 */
	rot = MODEL_ROTATION;
	rot.x *= dir;
	pModel->SetRot(rot.x, rot.y, rot.z);
	/*! 座標 */
	pModel->SetPos(pos.x, pos.y, pos.z);
}

/*!
	@brief	移動
*/
void Player::Move()
{
	switch (eMoveState)
	{
	case Player::WAIT:
		break;
	case Player::LEFT:
		pModel->SetMotionNum(1);
		MoveLeft();
		break;
	case Player::RIGHT:
		pModel->SetMotionNum(1);
		MoveRight();
		break;
	case Player::UP:
		MoveUp();
		break;
	case Player::DOWN:
		MoveDown();
		break;
	}
}

/*!
	@brief	左移動
*/
void Player::MoveLeft()
{
	/*! 移動中 */
	if (pos.x > nextPos.x) {
		
		/*! 毎フレーム移動加算量を増やしていく */
		additionValueBase += SPEED_ADD_VALUE;
		
		/*! 速度＝1フレーム辺りの移動量×移動加算量量を２乗した値 */
		speed = MOVE_PER_FRAME * powf(1 + additionValueBase, 2);

		/*! 速度上限に到達したら速度を丸める */
		speed = min(speed, MOVE_MAX_SPEED);

		/*! 移動 */
		pos.x -= speed;
	}
	/*! 移動終了 */
	else {
		index.x -= 1;					/*!< インデックス更新 */
		eMoveState = MoveState::WAIT;	/*!< 移動状態を待機へ */
		additionValueBase = 0;			/*!< 加算量の初期化 */
	}
}

/*!
	@brief	右移動
*/
void Player::MoveRight()
{	
	/*! 移動中 */
	if (pos.x < nextPos.x) {

		/*! 毎フレーム移動加算量を増やしていく */
		additionValueBase += SPEED_ADD_VALUE;

		/*! 速度＝1フレーム辺りの移動量×移動加算量量を２乗した値 */
		speed = MOVE_PER_FRAME * powf(1 + additionValueBase, 2);

		/*! 速度上限に到達したら速度を丸める */
		speed = min(speed, MOVE_MAX_SPEED);

		/*! 移動 */
		pos.x += speed;
	}
	/*! 移動終了 */
	else {
		index.x += 1;					/*!< インデックス更新 */
		eMoveState = MoveState::WAIT;	/*!< 移動状態を待機へ */
		additionValueBase = 0;			/*!< 加算量の初期化 */
	}

}

/*!
	@brief	上移動
	@detail	移動は等速直線移動
*/
void Player::MoveUp()
{
	if (pos.y < nextPos.y) 
	{
		pos.y += MOVE_PER_FRAME;
	}
	else {
		/*! 座標更新 */
		index.y -= 1;
		pos = nextPos;
		/*! パラメータリセット */
		eMoveState = MoveState::WAIT;	/*!< 移動状態を待機へ */
		isInput = false;				/*!< 入力済みフラグを戻す */
	}
}

/*!
	@brief	下移動
	@detail	移動は等速直線移動
*/
void Player::MoveDown()
{
	if (pos.y > nextPos.y)
	{
		pos.y -= MOVE_PER_FRAME;
	}
	else {
		/*! 座標更新 */
		index.y += 1;
		pos = nextPos;
		/*! パラメータリセット */
		eMoveState = MoveState::WAIT;	/*!< 移動状態を待機へ */
		isInput = false;				/*!< 入力済みフラグを戻す */
	}
}

/*!
	@brief	関数の遅延実行
	@detail	入力後アニメーションが規定フレームに到達するまで処理を待たせる
*/
void Player::Wait()
{
	/*! 関数が登録されてなければ処理をスキップ */
	if (delayFunc != nullptr) {

		/*! 規定フレームまで処理を保留 */
		if (waitCount++ <= WAIT_FRAME) { return; }

		/*! 初期化 */
		delayFunc();
		delayFunc = nullptr;
		waitCount = 0;
	}
}

/*!
	@brief	入力
*/
void Player::Input()
{
	/*****************************/
	/*		 ↓入力受付↓		 */
	/*****************************/
	
	/*! 入力済みなら処理を抜ける */
	if (isInput) { return; }

	/*! 移動状態なら処理を抜ける */
	if (eMoveState != MoveState::WAIT) { return; }

	/*****************************/
	/*			↓移動↓		 */
	/*****************************/

	/*! 左 */
	if (InputManager::GetInstance().GetLeft()) {
		dir = -1;/*!< 向きを左 */
		const INDEX nextIndex = { index.x + dir, index.y };
		if (!pRefInfo->GetIsBlock(nextIndex)){
			eMoveState = MoveState::LEFT;
			Convert(&nextPos, nextIndex);
		}
	}
	/*! 右 */
	if (InputManager::GetInstance().GetRight()) {
		dir = 1;/*!< 向きを右 */
		const INDEX nextIndex = { index.x + dir, index.y };
		if (!pRefInfo->GetIsBlock(nextIndex)) {
			eMoveState = MoveState::RIGHT;
			Convert(&nextPos, nextIndex);
		}
	}

	/*! プレイヤーの正面に動かすことが出来るブロック
	が無ければアクション出来ないので処理を抜ける */
	const INDEX front = { index.x + dir,index.y };

	if (!pRefInfo->GetIsMoveBlock(front)) { return; }

	/****************************/
	/*		↓アクション↓		*/
	/****************************/

	/*! 上 */
	if (InputManager::GetInstance().GetUpActionDown()) {

		/*! 対象のインデックスが移動できるかを判定し上移動できるかを判定 */
		const bool canMove =
			!pRefInfo->GetIsBlock({ index.x,index.y - 1 }) &&		/*!< プレイヤーの上 */
			!pRefInfo->GetIsBlock({ index.x + dir,index.y + 1 });	/*!< 対象の下 */

		/*! 移動先にブロックが存在していたのでアクションせずに処理を抜ける */
		if (!canMove) { return; }

		/***************************************************/
		/*! ココにアニメーションの再生処理を書いてください */
		/***************************************************/
		pModel->SetMotionNum(0);
		isInput = true;
		/***************************************************/

		/*! 遅延関数に処理を登録する */
		delayFunc = [&]{

			/*! 移動処理 */
			const INDEX nextIndex = { index.x, index.y - 1 };
			eMoveState = MoveState::UP;
			Convert(&nextPos, nextIndex);

			/*! ブロックの移動命令 */
			const INDEX bIndex = { index.x + dir,index.y };
			pRefInfo->UpAction(bIndex);

		};
	}

	/*! 下 */
	if (InputManager::GetInstance().GetDownActionDown()) {

		/*! 対象のインデックスが移動できるかを判定し上移動できるかを判定 */
		const bool canMove =
			!pRefInfo->GetIsBlock({ index.x,index.y + 1 }) &&		/*!< プレイヤーの下 */
			!pRefInfo->GetIsBlock({ index.x + dir,index.y - 1 });	/*!< 対象の上 */

		/*! 移動先にブロックが存在していたのでアクションせずに処理を抜ける */
		if (!canMove) { return; }

		/***************************************************/
		/*! ココにアニメーションの再生処理を書いてください */
		/***************************************************/
		pModel->SetMotionNum(0);
		isInput = true;
		/***************************************************/


		/*! 遅延関数に処理を登録する */
		delayFunc = [&] {

			/*! 移動処理 */
			const INDEX nextIndex = { index.x, index.y + 1 };
			eMoveState = MoveState::DOWN;
			Convert(&nextPos, nextIndex);

			/*! ブロックの移動命令 */
			const INDEX bIndex = { index.x + dir,index.y };
			pRefInfo->DownAction(bIndex);

		};
	}

}

/*!
	@brief	インデックスをXMFLOAT3の座標に変換
*/
void Player::Convert(DirectX::XMFLOAT3 * outPos, INDEX index)
{
	outPos->x = LEFT_TOP_OFFSET.x + index.x * OFFSET.x;
	outPos->y = LEFT_TOP_OFFSET.y - index.y * OFFSET.y;
	outPos->z = LEFT_TOP_OFFSET.z;

}