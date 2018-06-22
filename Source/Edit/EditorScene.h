/**
* @file		EditorScene.h
* @brief	�X�e�[�W�G�f�B�^�p�V�[��
* @author	�ԏ� �G�P
* @date�@	2018/05/29
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

/*! �X�e�[�W/�A�N�V�����p�Ńu���b�N���N���X���Ⴄ���ߗʑΉ�������\���̂��` */
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
	DirectX::XMINT2		iPos;/*!< �C���f�b�N�X */
	DirectX::XMFLOAT3	cPos;/*!< �J���� */
	umap_str_sprite pSprites;

	std::vector<std::vector<EditObj*>> objList;
};

