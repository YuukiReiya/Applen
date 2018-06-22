/*!
	@file	DataManager.cpp
	@date	2018/05/22
	@author	番場 宥輝
	@brief	データ管理クラス
*/
#include "DataManager.h"
#include "../StageLoader/StageLoader.h"
#include <fstream>
#include <iostream>

using namespace std;


/*!
	@def	定数宣言
*/
const string DataManager::sFilePath = "Resource/SaveData/SaveData.txt";
const string DataManager::sStage = "ステージ";

DataManager::DataManager()
{
}

DataManager::~DataManager()
{
}

/*!
	@brief	セーブデータのロード
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

	/*! 読み込んだ音量を変数にセット */
	SetVol();
	
	/*! ステージのクリア情報 */
	//for (int i = 1; i <= StageLoader::STAGE_NUM; ++i) {
	//	string no = sStage + to_string(i);
	//	if (loaded.find(no) == loaded.end()) { continue; }
	//	SetClearData(no, isClear[i]);
	//}
}

/*!
	@brief	現在の情報をセーブする
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
	@brief	音量の書き込み
*/
void DataManager::WriteVol(std::ofstream&ofs)
{
	ofs << "volume"<<endl;
	ofs << to_string(vol) << endl;
}

/*!
	@brief	クリアデータの書き込み
*/
void DataManager::WriteClearData(std::ofstream& ofs, std::string stage, bool clear)
{
	ofs << stage.c_str() << endl;
	std::string write = to_string(clear ? 1 : 0);
	ofs << write.c_str() << endl;
}

/*!
	@brief	ボリュームのセット
*/
void DataManager::SetVol()
{
	string key = "volume";
	vol = stof(loaded[key]);
}

/*!
	@brief	クリアデータのセット
*/
void DataManager::SetClearData(std::string stage, bool & out)
{
	string key = stage;
	out = stoi(loaded[key].c_str()) == 1;
}
