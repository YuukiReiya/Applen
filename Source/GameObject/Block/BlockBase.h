/*!
	@file	BlockBase.h
	@date	2018/04/09
	@author	番場 宥輝
	@brief	ブロックの基底クラス
*/
#pragma once
#include "../../Math/Index.h"
#include <Windows.h>
#include <DirectXMath.h>
#include <functional>

/*! 
	@brief	ブロックオブジェクトの基底クラス
*/

class BlockBase 
{
public:
	/*! コンストラクタ＆デストラクタ */
	BlockBase() {}
	BlockBase(INDEX index) { this->index = index; }
	virtual ~BlockBase() {}

	/*! 移動状態 */
	enum MoveState :int32_t {
		WAIT	= 0,	/*!< 待機 */
		UP,				/*!< 上移動中 */
		DOWN			/*!< 下移動中 */
	};

	/*! 公開関数 */
	virtual void Initialize();
	virtual void Finalize();
	virtual void StartMoveUp(std::function<void()> func = nullptr);
	virtual void StartMoveDown(std::function<void()> func = nullptr);

	/*! 純粋仮想関数 */
	virtual void Update() = 0;

	/*! 定数 */
	/*! ※IDの7桁目(最左)はアルファベットのため除いている */
	static const int ID_OPTION1;	/*!< 桁(IDの左から1文字目の桁を指定するオフセット) */
	static const int ID_OPTION2;	/*!< 桁(IDの左から2文字目の桁を指定するオフセット) */
	static const int ID_OPTION3;	/*!< 桁(IDの左から3文字目の桁を指定するオフセット) */
	static const int ID_OPTION4;	/*!< 桁(IDの左から4文字目の桁を指定するオフセット) */
	static const int ID_OPTION5;	/*!< 桁(IDの左から5文字目の桁を指定するオフセット) */
	static const int ID_OPTION6;	/*!< 桁(IDの左から6文字目の桁を指定するオフセット) */

	/*! セッター */
	void SetID(int id)			{ this->id = id; }
	void SetIndex(INDEX index)	{ this->index = index; }

	/*! ゲッター */
	DirectX::XMFLOAT3 GetPos()const { return from; }
	INDEX GetIndex()const		{ return index; }
	int GetID()const			{ return id; }
protected:
	/*! 被保護関数 */
	void Move();		/*!< 移動関数 */
	void MoveUp();		/*!< 上移動 */
	void MoveDown();	/*!< 下移動 */

	/*! 定数 */
	static const DirectX::XMFLOAT3 LEFT_TOP_POSITION;	/*!< 左上座標 */
	static const DirectX::XMFLOAT2 OFFSET;				/*!< オブジェクト間の幅 */
	static const float MOVE_RER_FRAME;					/*!< 1フレームあたりの移動量 */

	INDEX					index;					/*!< インデックス */
	int						id;						/*!< ID */
	MoveState				eState;					/*!< 移動状態 */
	DirectX::XMFLOAT3		from;					/*!< 移動元座標(現座標) */
	DirectX::XMFLOAT3		to;						/*!< 移動先座標 */
	std::function<void()>	actionFinishedFunc;		/*!< アクション終了時呼ばれるコールバック関数 */
};