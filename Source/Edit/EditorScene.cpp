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
	@brief	������
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
	@brief	�X�V
*/
SceneBase * EditorScene::Update(SceneManager * root)
{
	/*! �J�[�\���̈ړ� */
	MoveCursol();

	/*! �I�u�W�F�N�g�̔z�u */
	CreateObject();

	/*! �I�u�W�F�N�g�̃f���[�g */
	if (GetButtonDown(DELETE)) {

		int ret = 0;
		ret = MessageBoxA(NULL, "�I�u�W�F�N�g���폜���܂�", NULL, MB_OKCANCEL);

		if (ret == IDOK) {

			/*! �I�u�W�F�N�g�̍폜 */
			INDEX index = { iPos.x,abs(iPos.y) };

			if (objList[index.y][index.x] == NULL) {
				MessageBoxA(NULL, "�I�u�W�F�N�g�͊��ɂˁ[��I", NULL, MB_OKCANCEL);
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

	/*! �ۑ����ďI�� */
	if (GetButtonDown('\b')) {

		int ret = 0;
		ret = MessageBoxA(NULL, "�Z�[�u���ďI�����܂�", NULL, MB_OKCANCEL);

		if (ret == IDOK) {

			/*! �Z�[�u�f�[�^�������� */
			Write();

			/*! �I�� */
			exit(NULL);
		}

	}

	return this;
}

/*!
	@brief	�`��
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

	/*! �J�[�\�� */
	auto iTex = pSprites["POS"];
	iTex->Render();
}

/*!
	@brief	�j��
*/
void EditorScene::Destroy()
{
}

/*!
	@brief	�I�u�W�F�N�g�̐���
*/
void EditorScene::CreateObject()
{
	/*! �w�肵�Ă���C���f�b�N�X */
	INDEX index = { iPos.x,abs(iPos.y) };

	EditObj* node = NULL;

	/*! �ʏ� */
	if (GetButtonDown('N')) {
		NormalActionObject* nobj = new NormalActionObject(INDEX(index));
		nobj->Initialize();
		Add(nobj);

		node = new EditObj;
		node->action = nobj;
		node->isActionObj = true;
	}

	/*! ���� */
	if (GetButtonDown('L')) {
		LossActionObject* lobj = new LossActionObject(INDEX(index));
		lobj->Initialize();
		Add(lobj);

		node = new EditObj;
		node->action = lobj;
		node->isActionObj = true;
	}

	/*! ���� */
	if (GetButtonDown('R')) {
		ResonanceActionObject* robj = new ResonanceActionObject(INDEX(index));
		robj->Initialize();
		Add(robj);

		node = new EditObj;
		node->action = robj;
		node->isActionObj = true;

	}

	/*! �X�e�[�W */
	if (GetButtonDown('S')) {

	}

	/*! �ǉ� */
	if (node != NULL)
	{
		objList[index.y][index.x] = node;

	}
}

/*!
	@brief	�J�[�\���̈ړ�
*/
void EditorScene::MoveCursol()
{
	///*! �J�[�\���e�N�X�`���̃|�C���^ */
	//auto iTex = pSprites["POS"];

	///*********************************/
	///*!		�@�J�[�\������			*/
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
	//	/*! ��ԉ��̎��̑���𖳌��ɂ��� */
	//	iPos.y += (abs(iPos.y) < objList.size()) ? -1 : 0;
	//}
	//if (GetButtonDown(UP)) {

	//	/*! ��ԏ�̎��̑���𖳌��ɂ��� */
	//	iPos.y += iPos.y < 0 ? 1 : 0;
	//}
	//if (GetButtonDown('X')) {
	//	string str = "Index:\nx = " + to_string(abs(iPos.y)) + "\ny = " + to_string(objList.size());
	//	MessageBoxA(NULL, str.c_str(), NULL, MB_OK);
	//}


	///*********************************/
	///*!		�@ �J��������			*/
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
	@brief	�����o��
*/
void EditorScene::Write()
{
	ofstream data;

	data.open("Resource/Loaddata/StageData.csv", ios::trunc);

	/*! �\������ */
	for (int y = 0; y < objList.size(); ++y) {
		for (int x = 0; x < objList[y].size(); ++x) {


			if (objList[y][x] == NULL) {
				data << "N,";
				continue;
			}

			/*! �^���� */
			decltype(auto) type = typeid(objList[y][x]->action);

			if (objList[y][x]->isActionObj) {

				/*! �m�[�}�� */
				if (type == typeid(NormalActionObject*)) {
					data << "B100000";
				}
				/*! ���� */
				if (type == typeid(ResonanceActionObject)) {
					data << "R100000";
				}
				/*! ���� */
				if (type == typeid(LossActionObject)) {
					data << "C100000";
				}
			}
			else {
				data << "A100000";
			}
			data << ",";
		}
		/*! ���s */
		data << endl;
	}
	data << endl;

	/*! �v���C���[ */
	data << "player,Index.x,Index.y,Direction" << endl;
	data << endl;
	data << endl;

	/*! �J���� */
	data << "camera,pos.x,pos.y,pos.z" << endl;
	data << "," << cPos.x << "," << cPos.y << "," << cPos.z;
	data.close();
}

