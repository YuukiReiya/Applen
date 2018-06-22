#include "Sprite.h"
#include <fstream>
#include <sstream>

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* CSV�t�@�C����ǂݍ��݁A�f�[�^��map�Ɋi�[
* @param (path) CSV�t�@�C���̃p�X
* @return (sprite) �f�[�^���i�[����map��Ԃ�
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
umap_str_sprite Sprite::InitTex(const char* path)
{
	const int MAX_NUM = 30;
	const int MAX_INFO = 17;

	TEXTURE_DATA* pTD[MAX_NUM];

	int csv_size = 0;
	int data_size = 0;
	string str = "";
	string data[MAX_NUM][MAX_INFO];

	// CSV���J���ē��e��ǂݍ���
	ifstream ifs(path);

	if (!ifs)
	{
		MessageBox(0, L"CSV�t�@�C�����J���܂���", 0, MB_OK);
	}

	// �t�@�C���̒��g����s���ǂݎ��
	while (getline(ifs, str))
	{
		string tmp = "";
		istringstream stream(str);

		// ��؂蕶�����Ȃ��Ȃ�܂ŕ�������؂��Ă���
		while (getline(stream, tmp, ','))
		{
			// ��؂�ꂽ������tmp�ɓ���
			data[csv_size][data_size] = tmp;
			++data_size;
		}
		++csv_size;
		data_size = 0;
	}

	int i;

	// 1�s�ڂ����Ȃ̂�2�s�ڂ���f�[�^���i�[
	for (i = 0; i < csv_size; ++i, ++data_size)
	{
		pTD[data_size] = new TEXTURE_DATA;

		if (data[data_size][1].size() < 100)
		{
			// �z��̃T�C�Y�𒴂��Ȃ��ꍇ�ɂ̂ݏ�������
			std::char_traits<char>::copy(pTD[data_size]->filePath,
				data[i][1].c_str(),
				data[i][1].size() + 1);
		}
		pTD[data_size]->filePath[data[i][1].size() + 1];

		//�z��ɂ���f�[�^��float�ɕϊ�
		pTD[data_size]->xfPos.x = (float)atof(data[i][2].c_str());
		pTD[data_size]->xfPos.y = (float)atof(data[i][3].c_str());
		pTD[data_size]->xfPos.z = (float)atof(data[i][4].c_str());

		pTD[data_size]->xfRot.x = (float)atof(data[i][5].c_str());
		pTD[data_size]->xfRot.y = (float)atof(data[i][6].c_str());
		pTD[data_size]->xfRot.z = (float)atof(data[i][7].c_str());

		pTD[data_size]->xfScale.x = (float)atof(data[i][8].c_str());
		pTD[data_size]->xfScale.y = (float)atof(data[i][9].c_str());

		pTD[data_size]->xfSize.x = (float)atof(data[i][10].c_str());
		pTD[data_size]->xfSize.y = (float)atof(data[i][11].c_str());

		pTD[data_size]->xfColor.x = (float)atof(data[i][12].c_str());
		pTD[data_size]->xfColor.y = (float)atof(data[i][13].c_str());
		pTD[data_size]->xfColor.z = (float)atof(data[i][14].c_str());
		pTD[data_size]->xfColor.w = (float)atof(data[i][15].c_str());

		pTD[data_size]->isFollow = atoi(data[i][16].c_str());
	}

	//�ŏI�I�ɓ����R���e�i
	umap_str_sprite sprite;

	for (i = 1; i < data_size; ++i)
	{
		sprite[data[i][0]] = make_shared<SpriteBase>();
		if (FAILED(sprite[data[i][0]]->Init(pTD[i])))
		{
			break;
		}
	}

	return sprite;
}