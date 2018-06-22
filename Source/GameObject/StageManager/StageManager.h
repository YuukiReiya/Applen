/*!
	@file	StageManager.h
	@date	2018/04/05
	@author	�ԏ� �G�P
	@brief	�X�e�[�W�}�l�[�W���[
*/
#pragma once
#include "../../Math/Index.h"
#include "../Block/FixedBlock.h"
#include "../Block/NormalBlock.h"
#include "../Block/LossBlock.h"
#include "../Block/ResonanceBlock.h"
#include "../../Fbx/TakeModelData.h"
#include "../../Fbx/Texes/TexesTake.h"
#include <map>

/*!
	@def	�O���錾
*/
class SquaresManager;

/*!
	@brief	�u���b�N���Ǘ����X�e�[�W���\������N���X
*/
class StageManager
{
public:
	StageManager() {}
	~StageManager() {}

	void Initialize();
	void Finalize();
	void Render();
	void Update();

	/*! �Z�b�^�[ */
	void SetLoadData();
	void SetRefData(SquaresManager* ref);

	/*! �Q�b�^�[ */
	std::vector<NormalBlock*>GetNormalBlocks()const { return nBlocks; }
	std::vector<ResonanceBlock*>GetResonanceBlocks()const { return rBlocks; }
	std::vector<LossBlock*>GetLossBlocks()const { return lBlocks; }
	INDEX GetGoalIndex()const { return goalParam; }

private:
	/*! �Q�� */
	SquaresManager * pRef;

	/*! �萔 */
	static const DirectX::XMFLOAT3 GOAL_LEFT_TOP_POS;	/*!< �S�[���̍�����W */
	static const DirectX::XMFLOAT2 GOAL_OFFSET;			/*!< �S�[���C���f�b�N�X��1�ϓ����邱�Ƃŕω�����I�t�Z�b�g */

	/*! �p�����[�^�[ */
	INDEX goalParam;/*!< �S�[���̃C���f�b�N�X */

	/*! �I�u�W�F�N�g */
	std::vector<FixedBlock*>	fBlocks;/*!< �Œ� */
	std::vector<NormalBlock*>	nBlocks;/*!< �ʏ� */
	std::vector<ResonanceBlock*>rBlocks;/*!< ���� */
	std::vector<LossBlock*>		lBlocks;/*!< ���� */

	/*! ���ւ��p�e�N�X�`�� */
	TexesTake* pTexesTake;

	/*! �����_�[(���f��) */
	std::shared_ptr<TakeModelData> fBlockModel;	/*!< �Œ� */
	std::shared_ptr<TakeModelData> nBlockModel;	/*!< �ʏ� */
	std::shared_ptr<TakeModelData> lBlockModel;	/*!< ���� */
	std::shared_ptr<TakeModelData> rBlockModel;	/*!< ���� */
	std::shared_ptr<TakeModelData> goalModel;	/*!< �S�[�� */

	/*! �ϒ��z��ւ̒ǉ��p�Z�b�^�[ */
	void AddFBlock(int id,INDEX index);
	void AddNBlock(int id,INDEX index);
	void AddLBlock(int id,INDEX index);
	void AddRBlock(int id,INDEX index);
};