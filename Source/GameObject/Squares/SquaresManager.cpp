#include "SquaresManager.h"
#include "../../StageLoader/StageLoader.h"

/*!
	@brief	���O���
*/
using namespace std;

/*!
	@brief	������
*/
void SquaresManager::Initialize()
{
	/*! �ϒ��z��̏����� */
	DirectX::XMINT2 index = StageLoader::GetInstance().GetLineCSV();
	eGridState.resize(index.y);
	for (int y = 0; y < index.y; ++y) {
		eGridState[y].resize(index.x);
		for (auto&it : eGridState[y]) {
			it = GridState::NONE;
		}
	}

	/*! �p�����[�^������ */
	{
		isMovingBlock = false;
	}

	/****************************/
	/* ���e�I�u�W�F�N�g�̔z�u�� */
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
	@brief	���
*/
void SquaresManager::Finalize()
{
}

/*!
	�X�V
*/
#include "../../Input/Keyboard/Keyboard.h"
using namespace Keyboard;
void SquaresManager::Update()
{
	if (Keyboard::GetButtonDown('T')) {
		std::string str;
		//�f�o�b�O
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
	@brief	�f�o�b�O�p�̕`��@�\
*/
void SquaresManager::Render()
{
}
#endif // DebugDraw

/*!
	@brief		��ړ��A�N�V����
	@detail		�v���C���[�̏�ړ��ŌĂяo���A�u���b�N�̉��ړ����s��
	@param[in]	�u���b�N�̃C���f�b�N�X
*/
void SquaresManager::UpAction(INDEX index)
{
	try
	{
		/*! �͈̓`�F�b�N */
		if (index.y < 0 || eGridState.size() <= index.y)throw "y";
		if (index.x < 0 || eGridState[index.y].size() <= index.x)throw "x";
	}
	catch (const string&e)
	{
		string error = "\"UpAction\" function is out-of-range reference!\nindex is \"";
		error += e + "\"";
		assert(error.c_str());
	}

	

	/*!< ����ւ���C���f�b�N�X */
	const INDEX swapIndex = { index.x,index.y + 1 };

	/*! �G���[�`�F�b�N */
	if (eGridState[swapIndex.y][swapIndex.x] != GridState::NONE) {
		string error = "\"UpAction\" function on GridState is Invalid value";
		assert(error.c_str());/*!< NONE�ȊO�͕s���Ȓl�Ƃ���Assert���o�� */
	}

	/*! �O���b�h�̓���ւ����� */
	auto tmp = eGridState[index.y][index.x];
	eGridState[index.y][index.x] = eGridState[swapIndex.y][swapIndex.x];
	eGridState[swapIndex.y][swapIndex.x] = tmp;

	/*! �R�[���o�b�N�֐� */
	FPVOID func = nullptr;/*!< �ړ��I�����ɌĂ΂��֐� */

	/*! �u���b�N�̉��ړ� */
	switch (eGridState[swapIndex.y][swapIndex.x])
	{
	case SquaresManager::NONE:
		return;
	case SquaresManager::FIXED:
		return;
	case SquaresManager::NORMAL:
	{
		/*! �R�[���o�b�N�֐��̒��g */
		func = [&] {isMovingBlock = false; };

		/*! ���ړ����� */
		BlockDown(
			pStageRef->GetNormalBlocks(),
			index,
			func
		);
	}
		break;
	case SquaresManager::LOSS:
	{
		/*! �R�[���o�b�N�֐��̒��g */
		func = [&] {isMovingBlock = false; };

		/*! ���ړ����� */
		BlockDown(
			pStageRef->GetLossBlocks(),
			index,
			func
		);
	}
		break;
	case SquaresManager::RESONANCE:
	{
		/*! �R�[���o�b�N�֐��̒��g */
		func = [&] {isMovingBlock = false; };
		
		int id;

		/*! ���ߑΏۂ�T����ID���擾 */
		for (auto it : pStageRef->GetResonanceBlocks()) {

			/*! �C���f�b�N�X����v�����̂ŒT���Ώ� */
			if (it->GetIndex() == index) {
				id = it->GetID();
				break;
			}
		}
		
		///*! �����O���[�v�̂��̑S�Ă𓮂����邩�`�F�b�N */
		//for (auto it : pStageRef->GetResonanceBlocks()) {

		//	/*! ���ߑΏۂ̓X�L�b�v */
		//	if (it->GetIndex() == index) { continue; }

		//	/*! ���ߑΏۂƈႤ�O���[�v�̂��̂̓X�L�b�v */
		//	if (id / BlockBase::ID_OPTION1 != it->GetID() / BlockBase::ID_OPTION1) { continue; }

		//	INDEX i = it->GetIndex();
		//	i.y -= 1;

		//	/*! �ړ���Ƀu���b�N�����݂��Ă�����֐��𔲂��� */
		//	if (GetIsBlock(i)) {
		//		return;
		//	}
		//}

		/*! �����O���[�v�̂��̂𔽑΂ɓ����� */
		for (auto it : pStageRef->GetResonanceBlocks()) {

			/*! ���ߑΏۂ̓X�L�b�v */
			if (it->GetIndex() == index) { continue; }

			/*! ���ߑΏۂƈႤ�O���[�v�̂��̂̓X�L�b�v */
			if (id / BlockBase::ID_OPTION1 != it->GetID() / BlockBase::ID_OPTION1) { continue; }

			INDEX i = it->GetIndex();
			i.y -= 1;

			/*! �O���b�h�������������� */
			eGridState[it->GetIndex().y][it->GetIndex().x] = GridState::NONE;	/*!< �ړ��� */
			eGridState[i.y][i.x] = GridState::RESONANCE;				/*!< �ړ��� */

			/*! ��ړ����� */
			BlockUp(
				pStageRef->GetResonanceBlocks(),
				it->GetIndex(),
				nullptr
			);
		}

		/*! ���ړ����� */
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
	@brief		���ړ��A�N�V����
	@detail		�v���C���[�̉��ړ��ŌĂяo���A�u���b�N�̏�ړ����s��
	@param[in]	�u���b�N�̃C���f�b�N�X
*/
void SquaresManager::DownAction(INDEX index)
{
	try
	{
		/*! �͈̓`�F�b�N */
		if (index.y < 0 || eGridState.size() <= index.y)throw "y";
		if (index.x < 0 || eGridState[index.y].size() <= index.x)throw "x";
	}
	catch (const string&e)
	{
		string error = "\"UpAction\" function is out-of-range reference!\nindex is \"";
		error += e + "\"";
		assert(error.c_str());
	}

	/*!< ����ւ���C���f�b�N�X */
	const INDEX swapIndex = { index.x,index.y - 1 };

	/*! �G���[�`�F�b�N */
	if (eGridState[swapIndex.y][swapIndex.x] != GridState::NONE) {
		string error = "\"UpAction\" function on GridState is Invalid value";
		assert(error.c_str());/*!< NONE�ȊO�͕s���Ȓl�Ƃ���Assert���o�� */
	}

	/*! �O���b�h�̓���ւ����� */
	auto tmp = eGridState[index.y][index.x];
	eGridState[index.y][index.x] = eGridState[swapIndex.y][swapIndex.x];
	eGridState[swapIndex.y][swapIndex.x] = tmp;

	/*! �R�[���o�b�N�֐� */
	FPVOID func = nullptr;/*!< �ړ��I�����ɌĂ΂��֐� */

	/*! �u���b�N�̏�ړ� */
	switch (eGridState[swapIndex.y][swapIndex.x])
	{
	case SquaresManager::NONE:
		return;
	case SquaresManager::FIXED:
		return;
	case SquaresManager::NORMAL:
	{
		/*! �R�[���o�b�N�֐��̒��g */
		func = [&] {isMovingBlock = false; };

		/*! ��ړ����� */
		BlockUp(
			pStageRef->GetNormalBlocks(),
			index,
			func
		);
	}
		break;
	case SquaresManager::LOSS:
	{
		/*! �R�[���o�b�N�֐��̒��g */
		func = [&] {isMovingBlock = false; };

		/*! ��ړ����� */
		BlockUp(
			pStageRef->GetLossBlocks(),
			index,
			func
		);
	}
		break;
	case SquaresManager::RESONANCE:
	{
		/*! �R�[���o�b�N�֐��̒��g */
		func = [&] {isMovingBlock = false; };

		int id;

		/*! ���ߑΏۂ�T����ID���擾 */
		for (auto it : pStageRef->GetResonanceBlocks()) {

			/*! �C���f�b�N�X����v�����̂ŒT���Ώ� */
			if (it->GetIndex() == index) {
				id = it->GetID();
				break;
			}
		}

		///*! �����O���[�v�̂��̑S�Ă𓮂����邩�`�F�b�N */
		//for (auto it : pStageRef->GetResonanceBlocks()) {

		//	/*! ���ߑΏۂ̓X�L�b�v */
		//	if (it->GetIndex() == index) { continue; }

		//	/*! ���ߑΏۂƈႤ�O���[�v�̂��̂̓X�L�b�v */
		//	if (id / BlockBase::ID_OPTION1 != it->GetID() / BlockBase::ID_OPTION1) { continue; }

		//	INDEX i = it->GetIndex();
		//	i.y += 1;

		//	/*! �ړ���Ƀu���b�N�����݂��Ă�����֐��𔲂��� */
		//	if (GetIsBlock(i)) {
		//		return;
		//	}
		//}

		/*! �����O���[�v�̂��̂𔽑΂ɓ����� */
		for (auto it : pStageRef->GetResonanceBlocks()) {

			/*! ���ߑΏۂ̓X�L�b�v */
			if (it->GetIndex() == index) { continue; }

			/*! ���ߑΏۂƈႤ�O���[�v�̂��̂̓X�L�b�v */
			if (id / BlockBase::ID_OPTION1 != it->GetID() / BlockBase::ID_OPTION1) { continue; }

			INDEX i = it->GetIndex();
			i.y += 1;

			/*! �O���b�h�������������� */
			eGridState[it->GetIndex().y][it->GetIndex().x] = GridState::NONE;	/*!< �ړ��� */
			eGridState[i.y][i.x] = GridState::RESONANCE;				/*!< �ړ��� */

																				/*! ��ړ����� */
			BlockDown(
				pStageRef->GetResonanceBlocks(),
				it->GetIndex(),
				nullptr
			);
		}

		/*! ��ړ����� */
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
	@brief	�Q�Ƃ̐ݒ�
*/
void SquaresManager::SetRef(Player * player, StageManager * stageManager)
{
	pPlayerRef	= player;
	pStageRef	= stageManager;
}

/*!
	@brief		�u���b�N�̏�Ԃ�NONE(����)�ɕύX
	@detail		�����u���b�N�̏������X�e�[�g��ύX����������
	@param[in]	�ύX����C���f�b�N�X
*/
void SquaresManager::SetGridStateNone(INDEX index)
{
	eGridState[index.y][index.x] = GridState::NONE;
}

/*!
	@brief	�u���b�N���ړ����Ă��邩����
	@detail	�����o�ϐ���Ԃ��Ă��邾��
*/
bool SquaresManager::GetIsBlockMoving() const
{
	return isMovingBlock;
}

/*!
	@brief	�C���f�b�N�X��Ƀu���b�N�����݂��邩����
	@return	true:�u���b�N������,false:�u���b�N����
*/
bool SquaresManager::GetIsBlock(INDEX index) const
{
	try
	{
		/*! �͈̓`�F�b�N */
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
	@brief	�C���f�b�N�X��ɓ�������u���b�N�����݂��邩����
	@return	true:�u���b�N������,false:�u���b�N����
*/
bool SquaresManager::GetIsMoveBlock(INDEX index) const
{
	try
	{
		/*! �͈̓`�F�b�N */
		if (index.y < 0 || eGridState.size() <= index.y)throw exception();
		if (index.x < 0 || eGridState[index.y].size() <= index.x)throw exception();
	}
	catch (...)
	{
		return false;
	}

	bool ret = 
		eGridState[index.y][index.x] != GridState::NONE&&	/*!< �u���b�N���� */
		eGridState[index.y][index.x] != GridState::FIXED;	/*!< �Œ�u���b�N */

	return ret;
}

/*!
	@brief	�N���A����
	@detail	�v���C���[�̃C���f�b�N�X�ƃS�[���̃C���f�b�N�X���r���Ĉ�v���Ă��邩�𔻒�
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
	@brief		�u���b�N�̏�ړ����s��
	@detail		�u���b�N�̉ϒ��z����󂯎��A���̒�����C���f�b�N�X�ԍ��ɉ������u���b�N�̏�ړ����s��
	@param[in]	BlockBase���p�������N���X�̃C���X�^���X���������ϒ��z��(vector)
	@param[in]	�T���C���f�b�N�X
*/
template<typename UpBlocks>
void SquaresManager::BlockUp(UpBlocks blocks, INDEX index, std::function<void()>func)
{
	for (auto& it : blocks) {

		/*! �����u���b�N��NULL�ɂȂ��Ă���\�������邽��NULL�`�F�b�N���s�� */
		if (it == NULL) { continue; }

		if (it->GetIndex() == index) {
			it->StartMoveUp(func);
			return;
		}
	}
}

/*!
	@brief		�u���b�N�̉��ړ����s��
	@detail		�u���b�N�̉ϒ��z����󂯎��A���̒�����C���f�b�N�X�ԍ��ɉ������u���b�N�̉��ړ����s��
	@param[in]	BlockBase���p�������N���X�̃C���X�^���X���������ϒ��z��(vector)
	@param[in]	�T���C���f�b�N�X
*/
template<typename DownBlocks>
void SquaresManager::BlockDown(DownBlocks blocks, INDEX index, std::function<void()>func)
{
	for (auto& it : blocks) {

		/*! �����u���b�N��NULL�ɂȂ��Ă���\�������邽��NULL�`�F�b�N���s�� */
		if (it == NULL) { continue; }

		if (it->GetIndex() == index) {
			it->StartMoveDown(func);
			return;
		}
	}
}
