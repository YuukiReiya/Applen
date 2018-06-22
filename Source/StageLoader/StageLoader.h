/*!
	@file	StageLoader.h
	@date	2018/04/09
	@author	�ԏ� �G�P
	@brief	�X�e�[�W�f�[�^��.csv�ǂݍ���
	@detail	�e�N���X�ւ̎󂯓n���p�̍\���̂���`
*/
#pragma once
#include "../Math/Index.h"
#include <vector>
#include <map>
#include <DirectXMath.h>



/*!
	@brief	�u���b�N�̎�ޗ񋓑�
*/

enum BlockType :int8_t {
	BLOCK_NONE,			/*!< �u���b�N���� */
	BLOCK_FIXED,		/*!< �Œ� */
	BLOCK_NORMAL,		/*!< �ʏ� */
	BLOCK_LOSS,			/*!< ���� */
	BLOCK_RESONANCE,	/*!< ���� */
};

/*! 
	@brief	Info�\���̂̊��\����
*/
struct InfoBase {
	INDEX index;
};

/*! 
	@brief	�v���C���[�̏��\���� 
*/
struct PlayerInfo:
	public InfoBase
{
	int		dir;
};

/*!
	@brief	�J�����̏��\���� 
*/
struct CameraInfo
{
	DirectX::XMFLOAT3 pos;
};

/*!
	@brief	�u���b�N�̏��\����
*/
struct BlockInfo
	:public InfoBase
{
	BlockType type;
	int id;
};


/*!
	@brief	.csv����X�e�[�W�̃f�[�^��ǂݍ��ރN���X
*/
class StageLoader
{
public:
	~StageLoader();
	/*!
		@detail	.csv�̓ǂݍ��݉񐔂�}���邽�߂ɃV���O���g���ɂ��܂���
	*/
	static StageLoader&GetInstance() {
		static StageLoader instance;
		return instance;
	}
	void Initialize();
	void Finalize();
	bool DataLoad(int stageNo);

	/*! �萔 */
	static constexpr int STAGE_NUM = 20;

	/*! �Q�b�^�[ */
	int						GetLoadedStageNo()const { return loadStage; }
	DirectX::XMINT2			GetLineCSV()const		{ return csvSize; }			/*!< �ǂݍ���CSV�f�[�^�̍s/�� */
	std::vector<BlockInfo>	GetBlocksInfo()const	{ return vLoadBlocksInfo; }
	PlayerInfo				GetPlayerInfo()const	{ return loadPlayerInfo; }
	InfoBase				GetGoalInfo()const		{ return loadGoalInfo; }
	CameraInfo				GetCameraInfo()const	{ return loadCameraInfo; }
private:
	StageLoader();
	enum LoadState : char {
		NONE			= 'N',
		STAGE_OBJECT	= 'A',
		NORMAL_OBJECT	= 'B',
		LOSS_OBJECT		= 'C',
		RESONANCE_OBJECT= 'R',
		GOAL			= 'E',
	}eLoadState;
	int										loadStage;
	DirectX::XMINT2							csvSize;
	static const int INFO_OPTION1;
	static const int INFO_OPTION2;
	static const int INFO_OPTION3;
	static const int INFO_OPTION4;
	static const int INFO_OPTION5;
	static const int INFO_OPTION6;

	bool LoadFile(std::string csvPath);
	std::string FindStrings(std::string& sSourceString, std::string&& sFindString);
	int LineSizeCSV(std::string str);
	LoadState ConvertLoadState(std::string& id);
	
	std::vector<BlockInfo> vLoadBlocksInfo;
	PlayerInfo				loadPlayerInfo;
	InfoBase				loadGoalInfo;
	CameraInfo				loadCameraInfo;
};