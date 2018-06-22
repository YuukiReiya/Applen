/*!
	@file	DataManager.h
	@date	2018/05/22
	@author	番場 宥輝
	@brief	データ管理クラス
*/
#pragma once
#include <string>
#include <vector>
#include <map>

class DataManager
{
public:
	static DataManager&GetInstance() {
		static DataManager instance;
		return instance;
	}
	~DataManager();

	void Load();
	bool Save();
	void SetData(int stage, bool write) { isClear[stage - 1] = write; }
	void SetData(float vol) { DataManager::vol = vol; }
private:
	DataManager();
	float vol;
	bool isClear[20];

	std::map<std::string, std::string>loaded;

	void WriteVol(std::ofstream&ofs);
	void WriteClearData(std::ofstream& ofs,std::string stage, bool clear);
	/*! セッター */
	void SetVol();
	void SetClearData(std::string stage, bool& out);

	static const std::string sFilePath;
	static const std::string sStage;
};

