#include "StageLoader.h"
#include <fstream>
#include <sstream>
#include <assert.h>
#include <Windows.h>
#include "../Device/DeviceManager.h"

/*!
	@var	定数宣言
*/
const int StageLoader::INFO_OPTION1 = 100000;	/*!< 左から1文字目の桁を指定するオフセット */
const int StageLoader::INFO_OPTION2 = 10000;	/*!< 左から2文字目の桁を指定するオフセット */
const int StageLoader::INFO_OPTION3 = 1000;		/*!< 左から3文字目の桁を指定するオフセット */
const int StageLoader::INFO_OPTION4 = 100;		/*!< 左から4文字目の桁を指定するオフセット */
const int StageLoader::INFO_OPTION5 = 10;		/*!< 左から5文字目の桁を指定するオフセット */
const int StageLoader::INFO_OPTION6 = 1;		/*!< 左から6文字目の桁を指定するオフセット */

/*!
	@def	名前空間	
*/
using namespace std;

/*!
	@brief	コンストラクタ
*/
#include<algorithm>

StageLoader::StageLoader()
{
	SecureZeroMemory(this, sizeof(this));
	csvSize = { -1,-1 };
}

/*!
	@brief	デストラクタ
*/
StageLoader::~StageLoader()
{
	Finalize();
}

/*!
	@brief	初期化
*/
void StageLoader::Initialize()
{
	vLoadBlocksInfo.clear();
	loadStage = -1;
}
/*!
	@brief	ファイルのロード
*/

bool StageLoader::DataLoad(int stageNo)
{
	vLoadBlocksInfo.clear();

	this->loadStage = stageNo;

	string path = "Resource/LoadData/StageData" + to_string(stageNo) + ".csv";

	return LoadFile(path);
}
/*!
	@brief	ファイルのロード
*/
bool StageLoader::LoadFile(std::string csvPath)
{
	std::ifstream ifs(csvPath);
	if (!ifs) { return false; }/*!< ファイルのパスが違う */

	vector<string>data;		/*!< .csvから読み込んだデータを格納する可変長配列 */
	data.clear();			/*!< vectorの初期化 */
	data.shrink_to_fit();	/*!< vectorのメモリの初期化 */

	string sstream = "";/*!< 文字列ストリーム */

	/*! 定数式用に用いる一時変数 */
	int maxLine = 0;

	/*! 読み込んだファイルの文字列をセル毎に可変長配列に格納 */
	while (getline(ifs,sstream))
	{
		string tmp = sstream;
		maxLine= LineSizeCSV(tmp);
		int line = LineSizeCSV(tmp);
		/*! 1行毎の文字列を1セル毎に変換し格納 */
		for (int i = 0; i < line; i++)
		{
			auto cell = FindStrings(tmp, ",");
			data.push_back(cell);
		}
	}
	/*! オフセット計算に用いる定数式 */
	const int MAX_LINE = maxLine;	/*!< .csvの縦列の長さ */
	csvSize.x = MAX_LINE;			/*!<  */

	/*! エラーチェック */
	try
	{
		bool isLoadPlayerError = true;
		bool isLoadCameraError = true;
		for (auto it : data) {
			if (it == "player") { isLoadPlayerError = false; }
			if (it == "camera") { isLoadCameraError = false; }
		}
		if (isLoadPlayerError) {
			throw runtime_error(".csvからplayerを読み込むことが出来ませんでした。\n定義有無もしくはスペルミスを確認してください");
		}
		if (isLoadCameraError) {
			throw runtime_error(".csvからcameraを読み込むことが出来ませんでした。\n定義有無もしくはスペルミスを確認してください");
		}
	}
	catch (const std::exception&e)
	{
		MessageBoxA(NULL, e.what(), NULL, MB_OK);
		return false;
	}
	int index = 0;/*!< カウント */

	/*! ステージの読み込み処理 */
	for (std::string it = data[0]; it != "player"; it = data[index]) {
		int ix = index % MAX_LINE;/*!< 添字ｘ */
		int iy = index / MAX_LINE;/*!< 添字ｙ */

		eLoadState = ConvertLoadState(it);
		int readData = atoi(it.c_str());

		BlockInfo binfo;
		binfo.id = readData;
		binfo.index = { ix,iy };

		/*! 読み込んだ数値に対して振り分け */
		switch (eLoadState)
		{
			/*! ブロック無し */
		case StageLoader::NONE:
			break;
			/*! 固定ブロック */
		case StageLoader::STAGE_OBJECT:
			binfo.type = BlockType::BLOCK_FIXED;
			vLoadBlocksInfo.push_back(binfo);
			break;
			/*! 通常ブロック */
		case StageLoader::NORMAL_OBJECT:
			binfo.type = BlockType::BLOCK_NORMAL;
			vLoadBlocksInfo.push_back(binfo);
			break;
			/*! 消失ブロック */
		case StageLoader::LOSS_OBJECT:
			binfo.type = BlockType::BLOCK_LOSS;
			vLoadBlocksInfo.push_back(binfo);
			break;
			/*! ゴール */
		case StageLoader::GOAL:
			loadGoalInfo.index = { ix,iy };
			break;
			/*! 共鳴ブロック */
		case StageLoader::RESONANCE_OBJECT:
			binfo.type = BlockType::BLOCK_RESONANCE;
			vLoadBlocksInfo.push_back(binfo);
		default:
			break;
		}

		csvSize.y = iy;/*!< ループで更新されるが欲しいのはループが終わる瞬間のiyの値 */
		index++;
	}

	/*! プレイヤーとカメラの値が入っている位置のオフセット */
	const int PLAER_LOAD_OFFSET = 1 + MAX_LINE;
	const int CAMERA_LOAD_OFFSET = MAX_LINE - 4 + MAX_LINE + 1;/*!< 横の列のセル数- */

	index += PLAER_LOAD_OFFSET;/*!< インデックス更新 */

	/*! プレイヤーの読み込み処理 */
	loadPlayerInfo.index.x = atoi(data[index++].c_str());
	loadPlayerInfo.index.y = atoi(data[index++].c_str());
	loadPlayerInfo.dir = atoi(data[index++].c_str());

	index += CAMERA_LOAD_OFFSET;/*!< インデックス更新 */

	/*! カメラの読み込み処理 */
	loadCameraInfo.pos.x = static_cast<float>(atof(data[index++].c_str()));
	loadCameraInfo.pos.y = static_cast<float>(atof(data[index++].c_str()));
	loadCameraInfo.pos.z = static_cast<float>(atof(data[index++].c_str()));
	return true;
}

/*!
	@brief		文字列から対象文字列を検索、対象文字列までを返す
	@detail		ex).csvで","で区切りたい場合、","までの文字列を返すのでデータ型に関数を通し直接代入できる
	@param[out]	探し元の文字列
	@param[in]	探すべき文字列
	@return		対象文字列が見つかるまでの文字列
				見つからなかったら string::npos
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
	@brief		.csvの一行当たりのセル数 
	@param[in]	検索対象文字列
	@return		一行当たりのセル数を返す
*/
int StageLoader::LineSizeCSV(std::string str)
{
	/*! .csvは','(\t)で区切られている */
	int ret = 0;
	size_t linkPos = 0;
	while (linkPos != string::npos) {
		linkPos = str.find(",");
		str = str.substr(linkPos + 1);
		ret++;/*!< 文字列の中に含まれる","の数をカウントする */
	}
	return ret;/*!< 最終的に","の数がセルの数になる */
}

/*! 
	@brief			IDの先頭からLoadStateに変換
	@detail			IDからLoadStateを判断し判断されたものを返す※ID文字列は先頭アドレスを除いた整数型になる
	@param[in,out]	判定文字列(ID)
	@return			IDの先頭に入っている1文字をLoadStateに変換したもの
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
	@brief	解放
*/
void StageLoader::Finalize()
{
}
