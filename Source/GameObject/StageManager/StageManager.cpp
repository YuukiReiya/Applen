#include <locale>
#include "../../Fbx/TakeModelData.h"
#include "StageManager.h"
#include "../../StageLoader/StageLoader.h"
#include "../Squares/SquaresManager.h"

/*!
	@brief	名前空間
*/
using namespace DirectX;

/*!
	@def	定数宣言
*/

/*! ゴール左上座標 */
const XMFLOAT3 StageManager::GOAL_LEFT_TOP_POS = {
	-4.25f,/*!< x */
	3.0f,/*!< y */
	1,/*!< z */
};

/*! ゴールのオフセット */
const XMFLOAT2 StageManager::GOAL_OFFSET = {
	1.7f,/*!< x */
	1.7f,/*!< y */
};

/*!
	@brief	初期化
*/
void StageManager::Initialize()
{	
	/*! インスタンス初期化 */
	pTexesTake = new TexesTake;

	/*! 可変長配列の初期化 */
	fBlocks.clear();
	fBlocks.shrink_to_fit();
	nBlocks.clear();
	nBlocks.shrink_to_fit();
	lBlocks.clear();
	lBlocks.shrink_to_fit();
	rBlocks.clear();
	rBlocks.shrink_to_fit();

	/*! 読み込んだモデルを連想配列に登録 */
	nBlockModel = std::make_shared<TakeModelData>("normal");	
	fBlockModel = std::make_shared<TakeModelData>("stage");		
	lBlockModel = std::make_shared<TakeModelData>("loss");		
	rBlockModel = std::make_shared<TakeModelData>("resonance");	
	
	/*!< ゴール */
	goalModel = std::make_shared<TakeModelData>("goal");		

	/*! モデルの張替え用UV */
	pTexesTake->TexesCSVLoad("Resource/Model/uvBlock/LoadUVPath.csv");
}

/*!
	@brief	解放
*/
void StageManager::Finalize()
{
}

/*!
	@brief	更新
*/
void StageManager::Update()
{	
	/*! 固定 */
	for (auto& it : fBlocks) {
		it->Update();
	}
	/*! 通常 */
	for (auto& it : nBlocks) {
		it->Update();
	}
	/*! 消失 */
	for (auto& it : lBlocks) {

		/*! 削除済みならスキップ */
		if (it == NULL) { continue; }

		/*! 更新 */
		it->Update();

		/*! 削除フラグが立ってれば削除する */
		if (it->GetIsDelete()) {
			pRef->SetGridStateNone(it->GetIndex());	/*!< マス管理している方のデータを更新 */
			it = NULL;								/*!< 削除 */
		}
	}
	/*! 共鳴 */
	for (auto& it : rBlocks) {
		it->Update();
	}

}

/*!
	@brief	描画
*/
#ifdef DebugDraw
#include "../../Input/Keyboard/Keyboard.h"
using namespace Keyboard;
bool isRender = true;
#endif
void StageManager::Render()
{
#ifdef DebugDraw
	if (GetButtonDown(BACK)) { isRender = !isRender; }
	if (!isRender) { return; }
#endif
	/*! 固定 */
	for (auto& it : fBlocks) {
		fBlockModel->Get()->SetModelPos(it->GetPos());
		fBlockModel->Render();
	}
	/*! 通常 */
	for (auto& it : nBlocks) {
		nBlockModel->Get()->SetModelPos(it->GetPos());
		nBlockModel->Render();
	}
	/*! 消失 */
	for (auto& it : lBlocks) {
		if (it == NULL) { continue; }
		lBlockModel->Get()->SetModelPos(it->GetPos());
		lBlockModel->Get()->SetSkeltonValue(it->GetAlpha());
		lBlockModel->Render();
	}
	/*! 共鳴 */
	for (auto& it : rBlocks) {
		uint32_t no = it->GetID() / BlockBase::ID_OPTION1;		/*!< グループ番号取得 */
		rBlockModel->Get()->SetTexture(pTexesTake->GetTex(no));	/*!< グループ番号と同じ番号のテクスチャを貼り付ける */
		rBlockModel->Get()->SetModelPos(it->GetPos());			/*!< 座標値 */
		rBlockModel->Get()->SetModelRot(it->GetRot());			/*!< 回転値 */
		rBlockModel->Render();
	}

	/*! ステージがクリアされれば表示しない */
	if (!pRef->GetIsClear()) {

		XMFLOAT2 offPos;
		int ste = StageLoader::GetInstance().GetLoadedStageNo();

		if (ste >= 0)
		{
			offPos = { -0.5f, 0.0f };
		}
		if (ste >= 5)
		{
			offPos = { -0.4f, 0.0f };
		}
		if (ste >= 9)
		{
			offPos = { -0.1f, 0.0f };
		}
		if (ste >= 12 && ste <= 14)
		{
			offPos = { -0.3f, 0.0f };
		}
		if (ste >= 17)
		{
			offPos = { 0.4f, -0.2f };
		}
		if (ste >= 18)
		{
			offPos = { 0.25f, -0.1f };
		}

		/*! ゴール座標 */
		XMFLOAT3 pos = {
			GOAL_LEFT_TOP_POS.x + goalParam.x*GOAL_OFFSET.x + offPos.x,
			GOAL_LEFT_TOP_POS.y - goalParam.y*GOAL_OFFSET.y + offPos.y,
			GOAL_LEFT_TOP_POS.z
		};

		/*! ゴール描画 */
		goalModel->Get()->SetModelPos(pos);
		goalModel->Render();
	}
}

/*!
	@brief	ロードしたデータを設定
*/
void StageManager::SetLoadData()
{
	/*! オブジェクト毎に振り分け */
	for (auto&it : StageLoader::GetInstance().GetBlocksInfo()) {
		
		switch (it.type)
		{
		case BlockType::BLOCK_FIXED:
			AddFBlock(it.id, it.index);
			break;
		case BlockType::BLOCK_NORMAL:
			AddNBlock(it.id, it.index);
			break;
		case BlockType::BLOCK_LOSS:
			AddLBlock(it.id, it.index);
			break;
		case BlockType::BLOCK_RESONANCE:
			AddRBlock(it.id, it.index);
			break;
		default:
			break;
		}
	}
	
	/*! ゴール */
	goalParam = StageLoader::GetInstance().GetGoalInfo().index;
}

/*!
	@brief	参照の設定
*/
void StageManager::SetRefData(SquaresManager * ref)
{
	pRef = ref;
}

/********************************************************/
/*			switch文でインスタンス生成をして			*/
/*			vectorに格納するために関数化				*/
/********************************************************/

/*!
	@brief	固定ブロックの追加
*/
void StageManager::AddFBlock(int id, INDEX index)
{
	FixedBlock* block = new FixedBlock;
	block->SetID(id);
	block->SetIndex(index);
	block->Initialize();
	fBlocks.push_back(block);
}

/*!
	@brief	通常ブロックの追加
*/
void StageManager::AddNBlock(int id, INDEX index)
{
	NormalBlock* block = new NormalBlock;
	block->SetID(id);
	block->SetIndex(index);
	block->Initialize();
	nBlocks.push_back(block);
}

/*!
	@brief	消失ブロックの追加
*/
void StageManager::AddLBlock(int id, INDEX index)
{
	LossBlock* block = new LossBlock;
	block->SetID(id);
	block->SetIndex(index);
	block->Initialize();
	lBlocks.push_back(block);
}

/*!
	@brief	共鳴ブロックの追加
*/
void StageManager::AddRBlock(int id, INDEX index)
{
	ResonanceBlock* block = new ResonanceBlock;
	block->SetID(id);
	block->SetIndex(index);
	block->Initialize();
	rBlocks.push_back(block);
}