#include "EditorScene.h"
#include "../Input/Keyboard/Keyboard.h"
#include "../Device/DeviceManager.h"
#include "../Camera/CameraManager.h"
#include <fstream>

using namespace Keyboard;
using namespace DirectX;
using namespace std;

EditorScene::EditorScene()
{
	iPos = { 0,0 };
	cPos = { 0,0,-10 };
}


EditorScene::~EditorScene()
{
}

/*!
	@brief	初期化
*/
void EditorScene::Init()
{
	nobjr = std::make_shared<TakeModelData>("normal");
	robjr= std::make_shared<TakeModelData>("resonance");
	lobjr = std::make_shared<TakeModelData>("loss");
	sobjr = std::make_shared<TakeModelData>("stage");

	Sprite sprite;
	std::string csv;
	csv = "Resource/Texture/EditorTextures.csv";
	pSprites = move(sprite.InitTex(csv.c_str()));

	XMFLOAT3 cLook = cPos;
	cLook.z = 0;
	CameraManager::GetInstance().CameraSet(cPos, cLook);

	StageObject* obj;
	for (int y = 0; y < 6; ++y) {
		std::vector<EditObj*> tmpVec;
		EditObj* add = new EditObj;
		for (int x = 0; x < 6; ++x) {
			INDEX index = { x,y };
			obj = new StageObject(INDEX(index));
			obj->Initialize(index);
			add->isActionObj = false;
			add->stage = obj;
			tmpVec.push_back(add);
			Add(add->stage);
		}
		objList.push_back(tmpVec);
	}
	//std::vector<EditObj> tmpVec;
	//EditObj add;
	//INDEX index = { 0,6 };
	//obj = new StageObject(INDEX(index));
	//obj->Initialize(index);
	//add.isActionObj = false;
	//add.stage = obj;
	//tmpVec.push_back(add);
	//Add(*add.stage);
	//objList.push_back(tmpVec);
}

/*!
	@brief	更新
*/
SceneBase * EditorScene::Update(SceneManager * root)
{
	/*! カーソルの移動 */
	MoveCursol();

	/*! オブジェクトの配置 */
	CreateObject();

	/*! オブジェクトのデリート */
	if (GetButtonDown(DELETE)) {

		int ret = 0;
		ret = MessageBoxA(NULL, "オブジェクトを削除します", NULL, MB_OKCANCEL);

		if (ret == IDOK) {

			/*! オブジェクトの削除 */
			INDEX index = { iPos.x,abs(iPos.y) };

			if (objList[index.y][index.x] == NULL) {
				MessageBoxA(NULL, "オブジェクトは既にねーよ！", NULL, MB_OKCANCEL);
				return this;
			}

			if (objList[index.y][index.x]->isActionObj) {
				RemoveAObj(index);
			}
			else {
				RemoveSObj(index);
			}


		}
	}

	/*! 保存して終了 */
	if (GetButtonDown('\b')) {

		int ret = 0;
		ret = MessageBoxA(NULL, "セーブして終了します", NULL, MB_OKCANCEL);

		if (ret == IDOK) {

			/*! セーブデータ書き込み */
			Write();

			/*! 終了 */
			exit(NULL);
		}

	}

	return this;
}

/*!
	@brief	描画
*/
void EditorScene::Render()
{
	DeviceManager::GetInstance().SetNormalRender();

	for (auto& it : sobj) {
		if (it == NULL) { continue; }
		sobjr->Get()->SetModelPos(it->GetPos());
		sobjr->Render();
	}

	for (auto& it : nobj) {
		if (it == NULL) { continue; }
		nobjr->Get()->SetModelPos(it->GetPos());
		nobjr->Render();
	}

	for (auto& it : robj) {
		if (it == NULL) { continue; }
		robjr->Get()->SetModelPos(it->GetPos());
		robjr->Render();
	}

	for (auto& it : lobj) {
		if (it == NULL) { continue; }
		lobjr->Get()->SetModelPos(it->GetPos());
		lobjr->Render();
	}

	/*! カーソル */
	auto iTex = pSprites["POS"];
	iTex->Render();
}

/*!
	@brief	破棄
*/
void EditorScene::Destroy()
{
}

/*!
	@brief	オブジェクトの生成
*/
void EditorScene::CreateObject()
{
	/*! 指定しているインデックス */
	INDEX index = { iPos.x,abs(iPos.y) };

	EditObj* node = NULL;

	/*! 通常 */
	if (GetButtonDown('N')) {
		NormalActionObject* nobj = new NormalActionObject(INDEX(index));
		nobj->Initialize();
		Add(nobj);

		node = new EditObj;
		node->action = nobj;
		node->isActionObj = true;
	}

	/*! 消失 */
	if (GetButtonDown('L')) {
		LossActionObject* lobj = new LossActionObject(INDEX(index));
		lobj->Initialize();
		Add(lobj);

		node = new EditObj;
		node->action = lobj;
		node->isActionObj = true;
	}

	/*! 共鳴 */
	if (GetButtonDown('R')) {
		ResonanceActionObject* robj = new ResonanceActionObject(INDEX(index));
		robj->Initialize();
		Add(robj);

		node = new EditObj;
		node->action = robj;
		node->isActionObj = true;

	}

	/*! ステージ */
	if (GetButtonDown('S')) {

	}

	/*! 追加 */
	if (node != NULL)
	{
		objList[index.y][index.x] = node;

	}
}

/*!
	@brief	カーソルの移動
*/
void EditorScene::MoveCursol()
{
	///*! カーソルテクスチャのポインタ */
	//auto iTex = pSprites["POS"];

	///*********************************/
	///*!		　カーソル操作			*/
	///*********************************/

	//}
	//if (GetButtonDown('D')) {

	//	iPos.x += iPos.x < objList[-iPos.y].size() ? 1 : 0;

	//}
	//if (GetButtonDown('S')) {

	//	auto tmp = ConvertIndex(iPos);

	//	tmp.y += -1;

	//	//iPos.y += iPos.y > 0 ? -1 : 0;
	//	iPos.y += -1;

	//if (GetButtonDown(LEFT)) {

	//	iPos.x -= iPos.x > 0 ? 1 : 0;

	//}
	//if (GetButtonDown(RIGHT)) {

	//	if (abs(iPos.y) == objList.size()) {
	//		int tmp = objList.size();
	//		const int index = abs(tmp - 1);
	//		iPos.x += iPos.x < objList[index].size() ? 1 : 0;
	//	}
	//	else {
	//		iPos.x += iPos.x < objList[-iPos.y].size() ? 1 : 0;
	//	}
	//}
	//if (GetButtonDown(DOWN)) {

	//	//iPos.y += iPos.y < objList.size() ? 1 : 0;
	//	iPos.y += 1;
	//}
	//if (GetButtonDown('X')) {
	//	string str = "Index:\nx = " + to_string(iPos.x) + "\ny = " + to_string(iPos.y);
	//	/*! 一番下の時の操作を無効にする */
	//	iPos.y += (abs(iPos.y) < objList.size()) ? -1 : 0;
	//}
	//if (GetButtonDown(UP)) {

	//	/*! 一番上の時の操作を無効にする */
	//	iPos.y += iPos.y < 0 ? 1 : 0;
	//}
	//if (GetButtonDown('X')) {
	//	string str = "Index:\nx = " + to_string(abs(iPos.y)) + "\ny = " + to_string(objList.size());
	//	MessageBoxA(NULL, str.c_str(), NULL, MB_OK);
	//}


	///*********************************/
	///*!		　 カメラ操作			*/
	///*********************************/
	//const float CameraXpow = 0.02f;
	//const float CameraYpow = 0.02f;
	//const float CameraZpow = 0.02f;

	//if (GetButton('A')) {
	//	cPos.x -= CameraXpow;
	//}
	//if (GetButton('D')) {
	//	cPos.x += CameraXpow;
	//}
	//if (GetButton('W')) {
	//	cPos.y -= CameraYpow;
	//}
	//if (GetButton('S')) {
	//	cPos.y += CameraYpow;
	//}
	//if (GetButton('Q')) {
	//	cPos.z += CameraZpow;
	//}
	//if (GetButton('E')) {
	//	cPos.z -= CameraZpow;
	//}
	//XMFLOAT3 cLook = cPos;
	//cLook.z = 0;
	//CameraManager::GetInstance().CameraSet(cPos, cLook);
}

void EditorScene::Add(NormalActionObject* obj)
{
	nobj.push_back(obj);
}

void EditorScene::Add(ResonanceActionObject* obj)
{
	robj.push_back(obj);
}

void EditorScene::Add(LossActionObject* obj)
{
	lobj.push_back(obj);
}

void EditorScene::Add(StageObject* obj)
{
	sobj.push_back(obj);
}

void EditorScene::RemoveSObj(INDEX index)
{
	int no = 0;
	for (auto it : sobj) {
		
		if (it == NULL) { ++no; continue; }

		if (it->GetIndex() == index) {
			sobj[no] = NULL;
			break;
		}
		++no;
	}

	objList[index.y][index.x] = NULL;



}

void EditorScene::RemoveAObj(INDEX index)
{
	int no = 0;

	for (auto it : nobj) {

		if (it == NULL) { ++no; continue; }

		if (it->GetIndex() == index) {
			nobj[no] = NULL;
			break;
		}
		++no;
	}

	no = 0;
	for (auto it : robj) {
		if (it == NULL) { ++no; continue; }

		if (it->GetIndex() == index) {
			robj[no] = NULL;
			break;
		}
		++no;

	}

	no = 0;
	for (auto it : lobj) {
		if (it == NULL) { ++no; continue; }

		if (it->GetIndex() == index) {
			lobj[no] = NULL;
			break;
		}
		++no;

	}

	objList[index.y][index.x] = NULL;


}

/*! 
	@brief	書き出し
*/
void EditorScene::Write()
{
	ofstream data;

	data.open("Resource/Loaddata/StageData.csv", ios::trunc);

	/*! 構成部分 */
	for (int y = 0; y < objList.size(); ++y) {
		for (int x = 0; x < objList[y].size(); ++x) {


			if (objList[y][x] == NULL) {
				data << "N,";
				continue;
			}

			/*! 型判定 */
			decltype(auto) type = typeid(objList[y][x]->action);

			if (objList[y][x]->isActionObj) {

				/*! ノーマル */
				if (type == typeid(NormalActionObject*)) {
					data << "B100000";
				}
				/*! 共鳴 */
				if (type == typeid(ResonanceActionObject)) {
					data << "R100000";
				}
				/*! 消失 */
				if (type == typeid(LossActionObject)) {
					data << "C100000";
				}
			}
			else {
				data << "A100000";
			}
			data << ",";
		}
		/*! 改行 */
		data << endl;
	}
	data << endl;

	/*! プレイヤー */
	data << "player,Index.x,Index.y,Direction" << endl;
	data << endl;
	data << endl;

	/*! カメラ */
	data << "camera,pos.x,pos.y,pos.z" << endl;
	data << "," << cPos.x << "," << cPos.y << "," << cPos.z;
	data.close();
}

