#include <locale>
#include "../../Fbx/TakeModelData.h"
#include "StageManager.h"
#include "../../StageLoader/StageLoader.h"
#include "../Squares/SquaresManager.h"

/*!
	@brief	���O���
*/
using namespace DirectX;

/*!
	@def	�萔�錾
*/

/*! �S�[��������W */
const XMFLOAT3 StageManager::GOAL_LEFT_TOP_POS = {
	-4.25f,/*!< x */
	3.0f,/*!< y */
	1,/*!< z */
};

/*! �S�[���̃I�t�Z�b�g */
const XMFLOAT2 StageManager::GOAL_OFFSET = {
	1.7f,/*!< x */
	1.7f,/*!< y */
};

/*!
	@brief	������
*/
void StageManager::Initialize()
{	
	/*! �C���X�^���X������ */
	pTexesTake = new TexesTake;

	/*! �ϒ��z��̏����� */
	fBlocks.clear();
	fBlocks.shrink_to_fit();
	nBlocks.clear();
	nBlocks.shrink_to_fit();
	lBlocks.clear();
	lBlocks.shrink_to_fit();
	rBlocks.clear();
	rBlocks.shrink_to_fit();

	/*! �ǂݍ��񂾃��f����A�z�z��ɓo�^ */
	nBlockModel = std::make_shared<TakeModelData>("normal");	
	fBlockModel = std::make_shared<TakeModelData>("stage");		
	lBlockModel = std::make_shared<TakeModelData>("loss");		
	rBlockModel = std::make_shared<TakeModelData>("resonance");	
	
	/*!< �S�[�� */
	goalModel = std::make_shared<TakeModelData>("goal");		

	/*! ���f���̒��ւ��pUV */
	pTexesTake->TexesCSVLoad("Resource/Model/uvBlock/LoadUVPath.csv");
}

/*!
	@brief	���
*/
void StageManager::Finalize()
{
}

/*!
	@brief	�X�V
*/
void StageManager::Update()
{	
	/*! �Œ� */
	for (auto& it : fBlocks) {
		it->Update();
	}
	/*! �ʏ� */
	for (auto& it : nBlocks) {
		it->Update();
	}
	/*! ���� */
	for (auto& it : lBlocks) {

		/*! �폜�ς݂Ȃ�X�L�b�v */
		if (it == NULL) { continue; }

		/*! �X�V */
		it->Update();

		/*! �폜�t���O�������Ă�΍폜���� */
		if (it->GetIsDelete()) {
			pRef->SetGridStateNone(it->GetIndex());	/*!< �}�X�Ǘ����Ă�����̃f�[�^���X�V */
			it = NULL;								/*!< �폜 */
		}
	}
	/*! ���� */
	for (auto& it : rBlocks) {
		it->Update();
	}

}

/*!
	@brief	�`��
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
	/*! �Œ� */
	for (auto& it : fBlocks) {
		fBlockModel->Get()->SetModelPos(it->GetPos());
		fBlockModel->Render();
	}
	/*! �ʏ� */
	for (auto& it : nBlocks) {
		nBlockModel->Get()->SetModelPos(it->GetPos());
		nBlockModel->Render();
	}
	/*! ���� */
	for (auto& it : lBlocks) {
		if (it == NULL) { continue; }
		lBlockModel->Get()->SetModelPos(it->GetPos());
		lBlockModel->Get()->SetSkeltonValue(it->GetAlpha());
		lBlockModel->Render();
	}
	/*! ���� */
	for (auto& it : rBlocks) {
		uint32_t no = it->GetID() / BlockBase::ID_OPTION1;		/*!< �O���[�v�ԍ��擾 */
		rBlockModel->Get()->SetTexture(pTexesTake->GetTex(no));	/*!< �O���[�v�ԍ��Ɠ����ԍ��̃e�N�X�`����\��t���� */
		rBlockModel->Get()->SetModelPos(it->GetPos());			/*!< ���W�l */
		rBlockModel->Get()->SetModelRot(it->GetRot());			/*!< ��]�l */
		rBlockModel->Render();
	}

	/*! �X�e�[�W���N���A�����Ε\�����Ȃ� */
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

		/*! �S�[�����W */
		XMFLOAT3 pos = {
			GOAL_LEFT_TOP_POS.x + goalParam.x*GOAL_OFFSET.x + offPos.x,
			GOAL_LEFT_TOP_POS.y - goalParam.y*GOAL_OFFSET.y + offPos.y,
			GOAL_LEFT_TOP_POS.z
		};

		/*! �S�[���`�� */
		goalModel->Get()->SetModelPos(pos);
		goalModel->Render();
	}
}

/*!
	@brief	���[�h�����f�[�^��ݒ�
*/
void StageManager::SetLoadData()
{
	/*! �I�u�W�F�N�g���ɐU�蕪�� */
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
	
	/*! �S�[�� */
	goalParam = StageLoader::GetInstance().GetGoalInfo().index;
}

/*!
	@brief	�Q�Ƃ̐ݒ�
*/
void StageManager::SetRefData(SquaresManager * ref)
{
	pRef = ref;
}

/********************************************************/
/*			switch���ŃC���X�^���X����������			*/
/*			vector�Ɋi�[���邽�߂Ɋ֐���				*/
/********************************************************/

/*!
	@brief	�Œ�u���b�N�̒ǉ�
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
	@brief	�ʏ�u���b�N�̒ǉ�
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
	@brief	�����u���b�N�̒ǉ�
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
	@brief	���u���b�N�̒ǉ�
*/
void StageManager::AddRBlock(int id, INDEX index)
{
	ResonanceBlock* block = new ResonanceBlock;
	block->SetID(id);
	block->SetIndex(index);
	block->Initialize();
	rBlocks.push_back(block);
}