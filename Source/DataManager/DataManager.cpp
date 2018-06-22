/*!
	@file	DataManager.cpp
	@date	2018/05/22
	@author	�ԏ� �G�P
	@brief	�f�[�^�Ǘ��N���X
*/
#include "DataManager.h"
#include "../StageLoader/StageLoader.h"
#include <fstream>
#include <iostream>

using namespace std;


/*!
	@def	�萔�錾
*/
const string DataManager::sFilePath = "Resource/SaveData/SaveData.txt";
const string DataManager::sStage = "�X�e�[�W";

DataManager::DataManager()
{
}

DataManager::~DataManager()
{
}

/*!
	@brief	�Z�[�u�f�[�^�̃��[�h
*/
void DataManager::Load()
{
	ifstream ifs(sFilePath.c_str());

	if (!ifs) {
		return;
	}

	string sstream;

	while (getline(ifs,sstream))
	{
		string hash, data; 
		hash = sstream;
		getline(ifs, sstream);
		data = sstream;
		loaded[hash] = data;
	}

	/*! �ǂݍ��񂾉��ʂ�ϐ��ɃZ�b�g */
	SetVol();
	
	/*! �X�e�[�W�̃N���A��� */
	//for (int i = 1; i <= StageLoader::STAGE_NUM; ++i) {
	//	string no = sStage + to_string(i);
	//	if (loaded.find(no) == loaded.end()) { continue; }
	//	SetClearData(no, isClear[i]);
	//}
}

/*!
	@brief	���݂̏����Z�[�u����
*/
bool DataManager::Save()
{
	ofstream ofs(sFilePath);

	//if (!ofs) { return false; }

	WriteVol(ofs);

	//for (int i = 1; i <= StageLoader::STAGE_NUM; ++i) {
	//	string hash = sStage + to_string(i);
	//	WriteClearData(ofs, hash, isClear[i - 1]);
	//}
	return true;
}

/*!
	@brief	���ʂ̏�������
*/
void DataManager::WriteVol(std::ofstream&ofs)
{
	ofs << "volume"<<endl;
	ofs << to_string(vol) << endl;
}

/*!
	@brief	�N���A�f�[�^�̏�������
*/
void DataManager::WriteClearData(std::ofstream& ofs, std::string stage, bool clear)
{
	ofs << stage.c_str() << endl;
	std::string write = to_string(clear ? 1 : 0);
	ofs << write.c_str() << endl;
}

/*!
	@brief	�{�����[���̃Z�b�g
*/
void DataManager::SetVol()
{
	string key = "volume";
	vol = stof(loaded[key]);
}

/*!
	@brief	�N���A�f�[�^�̃Z�b�g
*/
void DataManager::SetClearData(std::string stage, bool & out)
{
	string key = stage;
	out = stoi(loaded[key].c_str()) == 1;
}
