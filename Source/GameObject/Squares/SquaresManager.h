/*!
	@file	GridManager.h
	@date	2018/04/05
	@author	�ԏ� �G�P
	@brief	�O���b�h�}�l�[�W���[
*/
#pragma once
#include <vector>
#include "../../Math/Index.h"
#include "../Player/Player.h"
#include "../StageManager/StageManager.h"


/*!
	@brief	�Q�[������}�X�Ǘ�����N���X
*/

/*! �f�o�b�O�`��p�}�N�� */
//#define DebugDraw

class SquaresManager 
{
public:
	void Initialize();
	void Finalize();
	void Update();
#ifdef DebugDraw
	void Render();
#endif
	void UpAction(INDEX index);
	void DownAction(INDEX index);

	/*! �Z�b�^�[ */
	void SetRef(Player* player, StageManager* stageManager);
	void SetGridStateNone(INDEX index);
	/*! �Q�b�^�[ */
	bool GetIsBlockMoving()const;			/*!< �u���b�N���ړ��������� */
	bool GetIsBlock(INDEX index)const;		/*!< �C���f�b�N�X��Ƀu���b�N�����݂��邩���� */
	bool GetIsMoveBlock(INDEX index)const;	/*!< �C���f�b�N�X��ɓ�������u���b�N�����݂��邩���� */
	bool GetIsClear()const;					/*!< �X�e�[�W�N���A�������ǂ���(�S�[���̃C���f�b�N�X�Ƀv���C���[�̃C���f�b�N�X�����邩) */
	bool GetIsLastStage()const;				/*!< �ŏI�X�e�[�W���ǂ������� */
private:
	template<typename UpBlocks>
	void BlockUp(UpBlocks blocks, INDEX index, std::function<void()>func);
	template<typename DownBlocks>
	void BlockDown(DownBlocks blocks, INDEX index, std::function<void()>func);

	/*! �񋓑� */
	enum GridState :int8_t {
		NONE,		/*!< �u���b�N���� */
		FIXED,		/*!< �Œ� */
		NORMAL,		/*!< �ʏ� */
		LOSS,		/*!< ���� */
		RESONANCE,	/*!< ���� */
	};

	/*! �ʖ��錾 */
	typedef std::function<void()> FPVOID;

	/*! �p�����[�^ */
	bool isMovingBlock;

	/*! �Q�� */
	Player*			pPlayerRef;			/*!< �v���C���[�̃|�C���^ */
	StageManager*	pStageRef;			/*!< �X�e�[�W�̃|�C���^ */

	/*! �ϒ��z�� */
	std::vector<std::vector<GridState>>	eGridState;	/*!< �O���b�h�̏�Ԃ��i�[ */
};