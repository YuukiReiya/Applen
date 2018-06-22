/**
* @file		EditorScene.h
* @brief	ステージエディタ用シーン
* @author	番場 宥輝
* @date　	2018/05/29
*/

#pragma once
#include "../Scene/Manager/SceneManager.h"
#include "../Sprite/Sprite.h"
#include "../GameObject/ActionObject/NormalActionObject.h"
#include "../GameObject/ActionObject/LossActionObject.h"
#include "../GameObject/ActionObject/ResonanceActionObject.h"
#include "../GameObject/StageObject/StageObject.h"
#include "../Fbx/TakeModelData.h"
#include <vector>
#include "../Sound/Wave/Wave.h"

/*! ステージ/アクション用でブロックをクラスが違うため量対応させる構造体を定義 */
struct EditObj {
	bool isActionObj;
	ActionObjectBase* action;
	StageObject* stage;
};

class EditorScene final : public Scene
{
public:
	EditorScene();
	~EditorScene();

	void Init();
	SceneBase* Update(SceneManager* root);
	void Render();
	void Destroy();

private:
	void CreateObject();
	void MoveCursol();

	void Add(NormalActionObject* obj);
	void Add(ResonanceActionObject* obj);
	void Add(LossActionObject* obj);
	void Add(StageObject* obj);
	void RemoveSObj(INDEX index);
	void RemoveAObj(INDEX index);
	void Write();

	std::vector<NormalActionObject*> nobj;
	std::vector<ResonanceActionObject*> robj;
	std::vector<LossActionObject*> lobj;
	std::vector<StageObject*>sobj;

	std::shared_ptr<TakeModelData> nobjr;
	std::shared_ptr<TakeModelData> robjr;
	std::shared_ptr<TakeModelData> lobjr;
	std::shared_ptr<TakeModelData> sobjr;
	DirectX::XMINT2		iPos;/*!< インデックス */
	DirectX::XMFLOAT3	cPos;/*!< カメラ */
	umap_str_sprite pSprites;

	std::vector<std::vector<EditObj*>> objList;
};

