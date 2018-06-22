/*!
	@file	SoundStorage.h
	@date	2018/05/25
	@author	�ԏ� �G�P
	@brief	�Q�[�����Ɏg���T�E���h�̃��X�g
*/
#pragma once
#include <map>
#include "../Wave/Wave.h"


class SoundStorage
{
public:
	static SoundStorage&GetInstance() {
		static SoundStorage instance;
		return instance;
	}
	~SoundStorage();

	Wave* GetWave(std::string hashKey) { return waveList[hashKey]; }
	void Initialize();
	void Finalize();
private:
	SoundStorage();
	std::map<std::string, std::string> keyList;
	std::map<std::string, Wave*>waveList;
};

