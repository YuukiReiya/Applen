#include "SquaresManager.h"
#include "../../StageLoader/StageLoader.h"

/*!
	@brief	名前空間
*/
using namespace std;

/*!
	@brief	初期化
*/
void SquaresManager::Initialize()
{
	/*! 可変長配列の初期化 */
	DirectX::XMINT2 index = StageLoader::GetInstance().GetLineCSV();
	eGridState.resize(index.y);
	for (int y = 0; y < index.y; ++y) {
		eGridState[y].resize(index.x);
		for (auto&it : eGridState[y]) {
			it = GridState::NONE;
		}
	}

	/*! パラメータ初期化 */
	{
		isMovingBlock = false;
	}

	/****************************/
	/* ↓各オブジェクトの配置↓ */
	/****************************/

	for (auto it : StageLoader::GetInstance().GetBlocksInfo()) {

		GridState state = GridState::NONE;

		switch (it.type)
		{
		case BLOCK_NONE:
			break;
		case BLOCK_FIXED:
			state = GridState::FIXED;
			break;
		case BLOCK_NORMAL:
			state = GridState::NORMAL;
			break;
		case BLOCK_LOSS:
			state = GridState::LOSS;
			break;
		case BLOCK_RESONANCE:
			state = GridState::RESONANCE;
			break;
		default:
			assert("Invalid value");
			break;
		}

		eGridState[it.index.y][it.index.x] = state;
	}
}

/*!
	@brief	解放
*/
void SquaresManager::Finalize()
{
}

/*!
	更新
*/
#include "../../Input/Keyboard/Keyboard.h"
using namespace Keyboard;
void SquaresManager::Update()
{
	if (Keyboard::GetButtonDown('T')) {
		std::string str;
		//デバッグ
		for (int i = 0; i < eGridState.size(); ++i) {
			for (int j = 0; j < eGridState[i].size(); ++j) {
				str += std::to_string(eGridState[i][j]) + ",";
			}
			str += "\n";
		}
		MessageBoxA(NULL, str.c_str(), NULL, MB_OK);
	}

}

#ifdef DebugDraw
/*!
	@brief	デバッグ用の描画機能
*/
void SquaresManager::Render()
{
}
#endif // DebugDraw

/*!
	@brief		上移動アクション
	@detail		プレイヤーの上移動で呼び出し、ブロックの下移動を行う
	@param[in]	ブロックのインデックス
*/
void SquaresManager::UpAction(INDEX index)
{
	try
	{
		/*! 範囲チェック */
		if (index.y < 0 || eGridState.size() <= index.y)throw "y";
		if (index.x < 0 || eGridState[index.y].size() <= index.x)throw "x";
	}
	catch (const string&e)
	{
		string error = "\"UpAction\" function is out-of-range reference!\nindex is \"";
		error += e + "\"";
		assert(error.c_str());
	}

	

	/*!< 入れ替え先インデックス */
	const INDEX swapIndex = { index.x,index.y + 1 };

	/*! エラーチェック */
	if (eGridState[swapIndex.y][swapIndex.x] != GridState::NONE) {
		string error = "\"UpAction\" function on GridState is Invalid value";
		assert(error.c_str());/*!< NONE以外は不正な値としてAssertを出す */
	}

	/*! グリッドの入れ替え処理 */
	auto tmp = eGridState[index.y][index.x];
	eGridState[index.y][index.x] = eGridState[swapIndex.y][swapIndex.x];
	eGridState[swapIndex.y][swapIndex.x] = tmp;

	/*! コールバック関数 */
	FPVOID func = nullptr;/*!< 移動終了時に呼ばれる関数 */

	/*! ブロックの下移動 */
	switch (eGridState[swapIndex.y][swapIndex.x])
	{
	case SquaresManager::NONE:
		return;
	case SquaresManager::FIXED:
		return;
	case SquaresManager::NORMAL:
	{
		/*! コールバック関数の中身 */
		func = [&] {isMovingBlock = false; };

		/*! 下移動命令 */
		BlockDown(
			pStageRef->GetNormalBlocks(),
			index,
			func
		);
	}
		break;
	case SquaresManager::LOSS:
	{
		/*! コールバック関数の中身 */
		func = [&] {isMovingBlock = false; };

		/*! 下移動命令 */
		BlockDown(
			pStageRef->GetLossBlocks(),
			index,
			func
		);
	}
		break;
	case SquaresManager::RESONANCE:
	{
		/*! コールバック関数の中身 */
		func = [&] {isMovingBlock = false; };
		
		int id;

		/*! 命令対象を探索しIDを取得 */
		for (auto it : pStageRef->GetResonanceBlocks()) {

			/*! インデックスが一致したので探索対象 */
			if (it->GetIndex() == index) {
				id = it->GetID();
				break;
			}
		}
		
		///*! 同じグループのもの全てを動かせるかチェック */
		//for (auto it : pStageRef->GetResonanceBlocks()) {

		//	/*! 命令対象はスキップ */
		//	if (it->GetIndex() == index) { continue; }

		//	/*! 命令対象と違うグループのものはスキップ */
		//	if (id / BlockBase::ID_OPTION1 != it->GetID() / BlockBase::ID_OPTION1) { continue; }

		//	INDEX i = it->GetIndex();
		//	i.y -= 1;

		//	/*! 移動先にブロックが存在していたら関数を抜ける */
		//	if (GetIsBlock(i)) {
		//		return;
		//	}
		//}

		/*! 同じグループのものを反対に動かす */
		for (auto it : pStageRef->GetResonanceBlocks()) {

			/*! 命令対象はスキップ */
			if (it->GetIndex() == index) { continue; }

			/*! 命令対象と違うグループのものはスキップ */
			if (id / BlockBase::ID_OPTION1 != it->GetID() / BlockBase::ID_OPTION1) { continue; }

			INDEX i = it->GetIndex();
			i.y -= 1;

			/*! グリッド情報を書き換える */
			eGridState[it->GetIndex().y][it->GetIndex().x] = GridState::NONE;	/*!< 移動元 */
			eGridState[i.y][i.x] = GridState::RESONANCE;				/*!< 移動先 */

			/*! 上移動命令 */
			BlockUp(
				pStageRef->GetResonanceBlocks(),
				it->GetIndex(),
				nullptr
			);
		}

		/*! 下移動命令 */
		BlockDown(
			pStageRef->GetResonanceBlocks(),
			index,
			func
		);
	}
		break;
	default:
		assert("Invalid value");
		break;
	}
	isMovingBlock = true;
}

/*!
	@brief		下移動アクション
	@detail		プレイヤーの下移動で呼び出し、ブロックの上移動を行う
	@param[in]	ブロックのインデックス
*/
void SquaresManager::DownAction(INDEX index)
{
	try
	{
		/*! 範囲チェック */
		if (index.y < 0 || eGridState.size() <= index.y)throw "y";
		if (index.x < 0 || eGridState[index.y].size() <= index.x)throw "x";
	}
	catch (const string&e)
	{
		string error = "\"UpAction\" function is out-of-range reference!\nindex is \"";
		error += e + "\"";
		assert(error.c_str());
	}

	/*!< 入れ替え先インデックス */
	const INDEX swapIndex = { index.x,index.y - 1 };

	/*! エラーチェック */
	if (eGridState[swapIndex.y][swapIndex.x] != GridState::NONE) {
		string error = "\"UpAction\" function on GridState is Invalid value";
		assert(error.c_str());/*!< NONE以外は不正な値としてAssertを出す */
	}

	/*! グリッドの入れ替え処理 */
	auto tmp = eGridState[index.y][index.x];
	eGridState[index.y][index.x] = eGridState[swapIndex.y][swapIndex.x];
	eGridState[swapIndex.y][swapIndex.x] = tmp;

	/*! コールバック関数 */
	FPVOID func = nullptr;/*!< 移動終了時に呼ばれる関数 */

	/*! ブロックの上移動 */
	switch (eGridState[swapIndex.y][swapIndex.x])
	{
	case SquaresManager::NONE:
		return;
	case SquaresManager::FIXED:
		return;
	case SquaresManager::NORMAL:
	{
		/*! コールバック関数の中身 */
		func = [&] {isMovingBlock = false; };

		/*! 上移動命令 */
		BlockUp(
			pStageRef->GetNormalBlocks(),
			index,
			func
		);
	}
		break;
	case SquaresManager::LOSS:
	{
		/*! コールバック関数の中身 */
		func = [&] {isMovingBlock = false; };

		/*! 上移動命令 */
		BlockUp(
			pStageRef->GetLossBlocks(),
			index,
			func
		);
	}
		break;
	case SquaresManager::RESONANCE:
	{
		/*! コールバック関数の中身 */
		func = [&] {isMovingBlock = false; };

		int id;

		/*! 命令対象を探索しIDを取得 */
		for (auto it : pStageRef->GetResonanceBlocks()) {

			/*! インデックスが一致したので探索対象 */
			if (it->GetIndex() == index) {
				id = it->GetID();
				break;
			}
		}

		///*! 同じグループのもの全てを動かせるかチェック */
		//for (auto it : pStageRef->GetResonanceBlocks()) {

		//	/*! 命令対象はスキップ */
		//	if (it->GetIndex() == index) { continue; }

		//	/*! 命令対象と違うグループのものはスキップ */
		//	if (id / BlockBase::ID_OPTION1 != it->GetID() / BlockBase::ID_OPTION1) { continue; }

		//	INDEX i = it->GetIndex();
		//	i.y += 1;

		//	/*! 移動先にブロックが存在していたら関数を抜ける */
		//	if (GetIsBlock(i)) {
		//		return;
		//	}
		//}

		/*! 同じグループのものを反対に動かす */
		for (auto it : pStageRef->GetResonanceBlocks()) {

			/*! 命令対象はスキップ */
			if (it->GetIndex() == index) { continue; }

			/*! 命令対象と違うグループのものはスキップ */
			if (id / BlockBase::ID_OPTION1 != it->GetID() / BlockBase::ID_OPTION1) { continue; }

			INDEX i = it->GetIndex();
			i.y += 1;

			/*! グリッド情報を書き換える */
			eGridState[it->GetIndex().y][it->GetIndex().x] = GridState::NONE;	/*!< 移動元 */
			eGridState[i.y][i.x] = GridState::RESONANCE;				/*!< 移動先 */

																				/*! 上移動命令 */
			BlockDown(
				pStageRef->GetResonanceBlocks(),
				it->GetIndex(),
				nullptr
			);
		}

		/*! 上移動命令 */
		BlockUp(
			pStageRef->GetResonanceBlocks(),
			index,
			func
		);
	}
		break;
	default:
		assert("Invalid value");
		break;
	}
	isMovingBlock = true;
}

/*!
	@brief	参照の設定
*/
void SquaresManager::SetRef(Player * player, StageManager * stageManager)
{
	pPlayerRef	= player;
	pStageRef	= stageManager;
}

/*!
	@brief		ブロックの状態をNONE(無し)に変更
	@detail		消失ブロックの消失時ステートを変更したかった
	@param[in]	変更するインデックス
*/
void SquaresManager::SetGridStateNone(INDEX index)
{
	eGridState[index.y][index.x] = GridState::NONE;
}

/*!
	@brief	ブロックが移動しているか判定
	@detail	メンバ変数を返しているだけ
*/
bool SquaresManager::GetIsBlockMoving() const
{
	return isMovingBlock;
}

/*!
	@brief	インデックス上にブロックが存在するか判定
	@return	true:ブロックが存在,false:ブロック無し
*/
bool SquaresManager::GetIsBlock(INDEX index) const
{
	try
	{
		/*! 範囲チェック */
		if (index.y < 0 || eGridState.size() <= index.y)throw exception();
		if (index.x < 0 || eGridState[index.y].size() <= index.x)throw exception();
	}
	catch (...)
	{
		return false;
	}
	bool ret = eGridState[index.y][index.x] != GridState::NONE;
	return ret;
}

/*!
	@brief	インデックス上に動かせるブロックが存在するか判定
	@return	true:ブロックが存在,false:ブロック無し
*/
bool SquaresManager::GetIsMoveBlock(INDEX index) const
{
	try
	{
		/*! 範囲チェック */
		if (index.y < 0 || eGridState.size() <= index.y)throw exception();
		if (index.x < 0 || eGridState[index.y].size() <= index.x)throw exception();
	}
	catch (...)
	{
		return false;
	}

	bool ret = 
		eGridState[index.y][index.x] != GridState::NONE&&	/*!< ブロック無し */
		eGridState[index.y][index.x] != GridState::FIXED;	/*!< 固定ブロック */

	return ret;
}

/*!
	@brief	クリア判定
	@detail	プレイヤーのインデックスとゴールのインデックスを比較して一致しているかを判定
*/
bool SquaresManager::GetIsClear() const
{
	return pPlayerRef->GetIndex() == pStageRef->GetGoalIndex();
}

bool SquaresManager::GetIsLastStage() const
{
	return false;
}

/*!
	@brief		ブロックの上移動を行う
	@detail		ブロックの可変長配列を受け取り、その中からインデックス番号に応じたブロックの上移動を行う
	@param[in]	BlockBaseを継承したクラスのインスタンスが入った可変長配列(vector)
	@param[in]	探索インデックス
*/
template<typename UpBlocks>
void SquaresManager::BlockUp(UpBlocks blocks, INDEX index, std::function<void()>func)
{
	for (auto& it : blocks) {

		/*! 消失ブロックはNULLになっている可能性があるためNULLチェックを行う */
		if (it == NULL) { continue; }

		if (it->GetIndex() == index) {
			it->StartMoveUp(func);
			return;
		}
	}
}

/*!
	@brief		ブロックの下移動を行う
	@detail		ブロックの可変長配列を受け取り、その中からインデックス番号に応じたブロックの下移動を行う
	@param[in]	BlockBaseを継承したクラスのインスタンスが入った可変長配列(vector)
	@param[in]	探索インデックス
*/
template<typename DownBlocks>
void SquaresManager::BlockDown(DownBlocks blocks, INDEX index, std::function<void()>func)
{
	for (auto& it : blocks) {

		/*! 消失ブロックはNULLになっている可能性があるためNULLチェックを行う */
		if (it == NULL) { continue; }

		if (it->GetIndex() == index) {
			it->StartMoveDown(func);
			return;
		}
	}
}
