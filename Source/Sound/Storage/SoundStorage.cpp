#include "SoundStorage.h"
#include <iostream>
#include <fstream>

using namespace std;

SoundStorage::SoundStorage()
{
}


SoundStorage::~SoundStorage()
{
}

/*! 
	@brief	������ 
*/
void SoundStorage::Initialize()
{
	const char* csvPath = "Resource/Sound/SoundList.csv";
	ifstream ifs(csvPath);

	if (!ifs) { return; }
	string sstream;

	/*! �ŏ���1�s�̓_�~�[�s */
	getline(ifs, sstream);

	size_t it;

	/*! .csv�ǂݍ��� */
	while (getline(ifs, sstream))
	{
		it = 0;
		while (it != string::npos)
		{
			it = sstream.find(",");
			string hash = sstream.substr(0, it);
			sstream = sstream.substr(it + 1);
			keyList[hash] = sstream;
		}
	}

	waveList.clear();
	map<string, Wave*>emptyList;
	waveList.swap(emptyList);

	/*! ���[�h */
	for (auto it : keyList) {

		waveList[it.first] = new Wave;
		//waveList[it.first]->SetWaveEndFunc([&] {waveList["ENTER"]->Play(); });

		waveList[it.first]->Load(it.second);
	}

	/*!  */
}
