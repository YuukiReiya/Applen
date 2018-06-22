/*!
	@file	Player.h
	@date	2018/04/04
	@author	番場 宥輝
	@brief	プレイヤー
*/
#pragma once
#include "../../Fbx/PlayerModel/PlayerModel.h"
#include "../../Math/Index.h"
#include <functional>

/*!
	@def	前方宣言
*/
class SquaresManager;

/*! 
	@brief	プレイヤークラス
*/
class Player
{
public:
	/*! コンストラクタ＆デストラクタ */
	Player();
	~Player();

	DirectX::XMFLOAT2 OFFSET;

	/*! 公開関数 */
	void Initialize();
	void Update();
	void Render();
	void Finalize();

	/*! セッター */
	void SetRef(SquaresManager* pRef) { pRefInfo = pRef; }
	void SetLoadData();
	/*! ゲッター */
	INDEX GetIndex()const { return index; }
private:
	void Move();
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();
	void Wait();
	void Input();
	void Convert(DirectX::XMFLOAT3* outPos,INDEX index);

	/*! 列挙体 */
	enum MoveState {
		WAIT,
		LEFT,
		RIGHT,
		UP,
		DOWN,
	}eMoveState;

	/*! 定数 */
	static const DirectX::XMFLOAT3 LEFT_TOP_OFFSET;
	static const DirectX::XMFLOAT3 MODEL_ROTATION;
	static const float  MOVE_PER_FRAME;
	static const float	MOVE_MAX_SPEED;
	static const float  SPEED_ADD_VALUE;
	static const int WAIT_FRAME;

	/*! パラメータ */
	PlayerModel*		pModel;				/*!< モデル */
	DirectX::XMFLOAT3	pos;				/*!< 座標 */
	DirectX::XMFLOAT3	rot;				/*!< 回転 */
	DirectX::XMFLOAT3	scale;				/*!< スケール */
	int					dir;				/*!< 方向 */
	INDEX				index;				/*!< インデックス */
	DirectX::XMFLOAT3	nextPos;			/*!< 移動先座標 */
	float				speed;				/*!< 速度 */
	float				additionValueBase;	/*!< 速度加算に用いる基礎量 */
	int					waitCount;			/*!< 待機カウンタ */
	bool				isInput;			/*!< 入力済みフラグ */

	/*! 参照 */
	SquaresManager*		pRefInfo;			/*!< マス管理のポインタ */

	/*! 遅延関数 */
	std::function<void()>delayFunc;			/*!< 遅延関数 */
};