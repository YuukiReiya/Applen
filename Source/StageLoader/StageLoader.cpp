#include "StageLoader.h"
#include <fstream>
#include <sstream>
#include <assert.h>
#include <Windows.h>
#include "../Device/DeviceManager.h"

/*!
	@var	�萔�錾
*/
const int StageLoader::INFO_OPTION1 = 100000;	/*!< ������1�����ڂ̌����w�肷��I�t�Z�b�g */
const int StageLoader::INFO_OPTION2 = 10000;	/*!< ������2�����ڂ̌����w�肷��I�t�Z�b�g */
const int StageLoader::INFO_OPTION3 = 1000;		/*!< ������3�����ڂ̌����w�肷��I�t�Z�b�g */
const int StageLoader::INFO_OPTION4 = 100;		/*!< ������4�����ڂ̌����w�肷��I�t�Z�b�g */
const int StageLoader::INFO_OPTION5 = 10;		/*!< ������5�����ڂ̌����w�肷��I�t�Z�b�g */
const int StageLoader::INFO_OPTION6 = 1;		/*!< ������6�����ڂ̌����w�肷��I�t�Z�b�g */

/*!
	@def	���O���	
*/
using namespace std;

/*!
	@brief	�R���X�g���N�^
*/
#include<algorithm>

StageLoader::StageLoader()
{
	SecureZeroMemory(this, sizeof(this));
	csvSize = { -1,-1 };
}

/*!
	@brief	�f�X�g���N�^
*/
StageLoader::~StageLoader()
{
	Finalize();
}

/*!
	@brief	������
*/
void StageLoader::Initialize()
{
	vLoadBlocksInfo.clear();
	loadStage = -1;
}
/*!
	@brief	�t�@�C���̃��[�h
*/

bool StageLoader::DataLoad(int stageNo)
{
	vLoadBlocksInfo.clear();

	this->loadStage = stageNo;

	string path = "Resource/LoadData/StageData" + to_string(stageNo) + ".csv";

	return LoadFile(path);
}
/*!
	@brief	�t�@�C���̃��[�h
*/
bool StageLoader::LoadFile(std::string csvPath)
{
	std::ifstream ifs(csvPath);
	if (!ifs) { return false; }/*!< �t�@�C���̃p�X���Ⴄ */

	vector<string>data;		/*!< .csv����ǂݍ��񂾃f�[�^���i�[����ϒ��z�� */
	data.clear();			/*!< vector�̏����� */
	data.shrink_to_fit();	/*!< vector�̃������̏����� */

	string sstream = "";/*!< ������X�g���[�� */

	/*! �萔���p�ɗp����ꎞ�ϐ� */
	int maxLine = 0;

	/*! �ǂݍ��񂾃t�@�C���̕�������Z�����ɉϒ��z��Ɋi�[ */
	while (getline(ifs,sstream))
	{
		string tmp = sstream;
		maxLine= LineSizeCSV(tmp);
		int line = LineSizeCSV(tmp);
		/*! 1�s���̕������1�Z�����ɕϊ����i�[ */
		for (int i = 0; i < line; i++)
		{
			auto cell = FindStrings(tmp, ",");
			data.push_back(cell);
		}
	}
	/*! �I�t�Z�b�g�v�Z�ɗp����萔�� */
	const int MAX_LINE = maxLine;	/*!< .csv�̏c��̒��� */
	csvSize.x = MAX_LINE;			/*!<  */

	/*! �G���[�`�F�b�N */
	try
	{
		bool isLoadPlayerError = true;
		bool isLoadCameraError = true;
		for (auto it : data) {
			if (it == "player") { isLoadPlayerError = false; }
			if (it == "camera") { isLoadCameraError = false; }
		}
		if (isLoadPlayerError) {
			throw runtime_error(".csv����player��ǂݍ��ނ��Ƃ��o���܂���ł����B\n��`�L���������̓X�y���~�X���m�F���Ă�������");
		}
		if (isLoadCameraError) {
			throw runtime_error(".csv����camera��ǂݍ��ނ��Ƃ��o���܂���ł����B\n��`�L���������̓X�y���~�X���m�F���Ă�������");
		}
	}
	catch (const std::exception&e)
	{
		MessageBoxA(NULL, e.what(), NULL, MB_OK);
		return false;
	}
	int index = 0;/*!< �J�E���g */

	/*! �X�e�[�W�̓ǂݍ��ݏ��� */
	for (std::string it = data[0]; it != "player"; it = data[index]) {
		int ix = index % MAX_LINE;/*!< �Y���� */
		int iy = index / MAX_LINE;/*!< �Y���� */

		eLoadState = ConvertLoadState(it);
		int readData = atoi(it.c_str());

		BlockInfo binfo;
		binfo.id = readData;
		binfo.index = { ix,iy };

		/*! �ǂݍ��񂾐��l�ɑ΂��ĐU�蕪�� */
		switch (eLoadState)
		{
			/*! �u���b�N���� */
		case StageLoader::NONE:
			break;
			/*! �Œ�u���b�N */
		case StageLoader::STAGE_OBJECT:
			binfo.type = BlockType::BLOCK_FIXED;
			vLoadBlocksInfo.push_back(binfo);
			break;
			/*! �ʏ�u���b�N */
		case StageLoader::NORMAL_OBJECT:
			binfo.type = BlockType::BLOCK_NORMAL;
			vLoadBlocksInfo.push_back(binfo);
			break;
			/*! �����u���b�N */
		case StageLoader::LOSS_OBJECT:
			binfo.type = BlockType::BLOCK_LOSS;
			vLoadBlocksInfo.push_back(binfo);
			break;
			/*! �S�[�� */
		case StageLoader::GOAL:
			loadGoalInfo.index = { ix,iy };
			break;
			/*! ���u���b�N */
		case StageLoader::RESONANCE_OBJECT:
			binfo.type = BlockType::BLOCK_RESONANCE;
			vLoadBlocksInfo.push_back(binfo);
		default:
			break;
		}

		csvSize.y = iy;/*!< ���[�v�ōX�V����邪�~�����̂̓��[�v���I���u�Ԃ�iy�̒l */
		index++;
	}

	/*! �v���C���[�ƃJ�����̒l�������Ă���ʒu�̃I�t�Z�b�g */
	const int PLAER_LOAD_OFFSET = 1 + MAX_LINE;
	const int CAMERA_LOAD_OFFSET = MAX_LINE - 4 + MAX_LINE + 1;/*!< ���̗�̃Z����- */

	index += PLAER_LOAD_OFFSET;/*!< �C���f�b�N�X�X�V */

	/*! �v���C���[�̓ǂݍ��ݏ��� */
	loadPlayerInfo.index.x = atoi(data[index++].c_str());
	loadPlayerInfo.index.y = atoi(data[index++].c_str());
	loadPlayerInfo.dir = atoi(data[index++].c_str());

	index += CAMERA_LOAD_OFFSET;/*!< �C���f�b�N�X�X�V */

	/*! �J�����̓ǂݍ��ݏ��� */
	loadCameraInfo.pos.x = static_cast<float>(atof(data[index++].c_str()));
	loadCameraInfo.pos.y = static_cast<float>(atof(data[index++].c_str()));
	loadCameraInfo.pos.z = static_cast<float>(atof(data[index++].c_str()));
	return true;
}

/*!
	@brief		�����񂩂�Ώە�����������A�Ώە�����܂ł�Ԃ�
	@detail		ex).csv��","�ŋ�؂肽���ꍇ�A","�܂ł̕������Ԃ��̂Ńf�[�^�^�Ɋ֐���ʂ����ڑ���ł���
	@param[out]	�T�����̕�����
	@param[in]	�T���ׂ�������
	@return		�Ώە����񂪌�����܂ł̕�����
				������Ȃ������� string::npos
*/
std::string StageLoader::FindStrings(std::string & sSourceString, std::string && sFindString)
{
	size_t findIndex = 0;
	findIndex = sSourceString.find(",");
	auto ret = sSourceString.substr(0, findIndex);
	sSourceString = sSourceString.substr(findIndex + 1);
	return ret;
}

/*! 
	@brief		.csv�̈�s������̃Z���� 
	@param[in]	�����Ώە�����
	@return		��s������̃Z������Ԃ�
*/
int StageLoader::LineSizeCSV(std::string str)
{
	/*! .csv��','(\t)�ŋ�؂��Ă��� */
	int ret = 0;
	size_t linkPos = 0;
	while (linkPos != string::npos) {
		linkPos = str.find(",");
		str = str.substr(linkPos + 1);
		ret++;/*!< ������̒��Ɋ܂܂��","�̐����J�E���g���� */
	}
	return ret;/*!< �ŏI�I��","�̐����Z���̐��ɂȂ� */
}

/*! 
	@brief			ID�̐擪����LoadState�ɕϊ�
	@detail			ID����LoadState�𔻒f�����f���ꂽ���̂�Ԃ���ID������͐擪�A�h���X�������������^�ɂȂ�
	@param[in,out]	���蕶����(ID)
	@return			ID�̐擪�ɓ����Ă���1������LoadState�ɕϊ���������
*/
StageLoader::LoadState StageLoader::ConvertLoadState(std::string & id)
{
	LoadState ret;
	if (id.empty()) { return LoadState::NONE; }
	ret = static_cast<LoadState>(id[0]);
	id = id.substr(1);
	return ret;
}

/*!
	@brief	���
*/
void StageLoader::Finalize()
{
}
