/*!
	@file	StageLoader.h
	@date	2018/04/09
	@author	番場 宥輝
	@brief	ステージデータの.csv読み込み
	@detail	各クラスへの受け渡し用の構造体も定義
*/
#pragma once
#include "../Math/Index.h"
#include <vector>
#include <map>
#include <DirectXMath.h>



/*!
	@brief	ブロックの種類列挙体
*/

enum BlockType :int8_t {
	BLOCK_NONE,			/*!< ブロック無し */
	BLOCK_FIXED,		/*!< 固定 */
	BLOCK_NORMAL,		/*!< 通常 */
	BLOCK_LOSS,			/*!< 消失 */
	BLOCK_RESONANCE,	/*!< 共鳴 */
};

/*! 
	@brief	Info構造体の基底構造体
*/
struct InfoBase {
	INDEX index;
};

/*! 
	@brief	プレイヤーの情報構造体 
*/
struct PlayerInfo:
	public InfoBase
{
	int		dir;
};

/*!
	@brief	カメラの情報構造体 
*/
struct CameraInfo
{
	DirectX::XMFLOAT3 pos;
};

/*!
	@brief	ブロックの情報構造体
*/
struct BlockInfo
	:public InfoBase
{
	BlockType type;
	int id;
};


/*!
	@brief	.csvからステージのデータを読み込むクラス
*/
class StageLoader
{
public:
	~StageLoader();
	/*!
		@detail	.csvの読み込み回数を抑えるためにシングルトンにしました
	*/
	static StageLoader&GetInstance() {
		static StageLoader instance;
		return instance;
	}
	void Initialize();
	void Finalize();
	bool DataLoad(int stageNo);

	/*! 定数 */
	static constexpr int STAGE_NUM = 20;

	/*! ゲッター */
	int						GetLoadedStageNo()const { return loadStage; }
	DirectX::XMINT2			GetLineCSV()const		{ return csvSize; }			/*!< 読み込んだCSVデータの行/列数 */
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