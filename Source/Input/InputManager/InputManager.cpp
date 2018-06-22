/*!
	@file	InputManager.h
	@date	2018/04/13
	@author	番場 宥輝
	@brief	入力管理クラス
*/
#include "InputManager.h"
#include "../Keyboard/Keyboard.h"

/*!
	@brief	名前空間
*/
using namespace Keyboard;
using namespace KeyCode;

/*!
	@def	定数宣言
*/
const int InputManager::RESET_FRAME = 15;

/*!
	@brief	コンストラクタ
*/
InputManager::InputManager()
{
}

/*!
	@brief	デストラクタ
*/
InputManager::~InputManager()
{
	Finalize();
}

/*!
	@brief	初期化
*/
void InputManager::Initialize()
{
	/*! 1Pコントローラーを使う */
	pGamePad = new GamePad(GamePad::Index::One);
	/*! 連続入力防止用フラグ */
	isInput = 0;
}

/*!
	@brief	入力バッファ更新
*/
void InputManager::Update()
{
	Keyboard::Update();
	pGamePad->Update();
}

/*!
	@brief	解放
*/
void InputManager::Finalize()
{
	if (!pGamePad) {
		delete(pGamePad);
		pGamePad = nullptr;
	}
}

/*!
	@brief	左が入力されている間
*/
bool InputManager::GetLeft()
{
	/*! ゲームパッドの左ジョイスティック */
	if (pGamePad->GetJoyStick(JoyStick::LEFT).x < 0) {
		return true;
	}

	/*! ゲームパッドの十字キー */
	if (pGamePad->GetArrow(Arrow::HORIZONTAL) < 0) {
		return true;
	}

	/*! キーボード */
	if (GetButton(Keyboard::LEFT)) {
		return true;
	}

	return false;
}

/*!
	@brief	左が入力された瞬間
*/
bool InputManager::GetLeftDown()
{
	/*! ゲームパッドの左ジョイスティック */
	if (pGamePad->GetJoyStick(JoyStick::LEFT).x < 0.0f) {

		/*! マスクにヒットしなければ処理せずスキップ */
		bool isSkip = (isInput & 0b0100) != 0;
		if (isSkip) { return false; }

		/*! マスクを立てる */
		isInput = (isInput | 0b0100);
		return true;
	}

	/*! ゲームパッドの十字キー */
	if (pGamePad->GetArrowDown(Arrow::HORIZONTAL) < 0) {
		return true;
	}

	/*! キーボード */
	if (Keyboard::GetButtonDown(Keyboard::LEFT)) {
		return true;
	}

	/*! 入力マスクのリセット */
	isInput = (isInput & 0b11111011);

	return false;
}

/*!
	@brief	右が入力されている間
*/

bool InputManager::GetRight()
{
	/*! ゲームパッドの左ジョイスティック */
	if (pGamePad->GetJoyStick(JoyStick::LEFT).x > 0) {
		return true;
	}

	/*! ゲームパッドの十字キー */
	if (pGamePad->GetArrow(Arrow::HORIZONTAL) > 0) {
		return true;
	}

	/*! キーボード */
	if (GetButton(Keyboard::RIGHT)) {
		return true;
	}

	return false;
}

/*!
	@brief	右が入力された瞬間
*/
bool InputManager::GetRightDown()
{
	/*! ゲームパッドの左ジョイスティック */
	if (pGamePad->GetJoyStick(JoyStick::LEFT).x > 0.0f) {

		/*! マスクにヒットしなければ処理せずスキップ */
		bool isSkip = (isInput & 0b1000) != 0;
		if (isSkip) { return false; }

		/*! マスクを立てる */
		isInput = (isInput | 0b1000);
		return true;
	}

	/*! ゲームパッドの十字キー */
	if (pGamePad->GetArrowDown(Arrow::HORIZONTAL) > 0) {
		return true;
	}

	/*! キーボード */
	if (Keyboard::GetButtonDown(Keyboard::RIGHT)) {
		return true;
	}

	/*! 入力マスクのリセット */
	isInput = (isInput & 0b11110111);

	return false;
}

/*!
	@brief	STARTキーを押した瞬間
*/
bool InputManager::GetStartDown()
{
	if (pGamePad->GetButtonDown(Button::START) ||
		Keyboard::GetButtonDown(Keyboard::SPACE)) {
		return true;
	}

	return false;
}

/*!
	@brief	アクション1ボタンの押した瞬間
*/
bool InputManager::GetUpActionDown()
{
	if (pGamePad->GetButtonDown(Button::Y) ||
		Keyboard::GetButtonDown(Keyboard::UP)) {
		return true;
	}
	return false;
}

/*!
	@brief	アクション2ボタンの押した瞬間
*/
bool InputManager::GetDownActionDown()
{
	if (pGamePad->GetButtonDown(Button::A) ||
		Keyboard::GetButtonDown(Keyboard::DOWN)) {
		return true;
	}
	return false;
}

/*!
	@brief	上ボタン入力
*/
bool InputManager::GetUpDown()
{

	/*! ゲームパッドの左ジョイスティック */
	if (pGamePad->GetJoyStick(JoyStick::LEFT).y > 0.0f) {

		/*! マスクにヒットしなければ処理せずスキップ */
		bool isSkip = (isInput & 0b0001) != 0;
		if (isSkip) { return false; }

		/*! マスクを立てる */
		isInput = (isInput | 0b0001);
		return true;
	}

	/*! ゲームパッドの十字キー */
	if (pGamePad->GetArrowDown(Arrow::VERTICAL) > 0) {
		return true;
	}

	/*! キーボード */
	if (GetButtonDown(Keyboard::UP)) {
		return true;
	}

	/*! 入力マスクのリセット */
	isInput = (isInput & 0b11111110);

	return false;
}

/*!
	@brief	下ボタン入力
*/
bool InputManager::GetUnderDown()
{

	/*! ゲームパッドの左ジョイスティック */
	if (pGamePad->GetJoyStick(JoyStick::LEFT).y < 0) {
		
		/*! マスクにヒットしなければ処理せずスキップ */
		bool isSkip = (isInput & 0b0010) != 0;
		if (isSkip) { return false; }

		/*! マスクを立てる */
		isInput = (isInput | 0b0010);
		return true;


	}

	/*! ゲームパッドの十字キー */
	if (pGamePad->GetArrowDown(Arrow::VERTICAL) < 0) {
		return true;
	}

	/*! キーボード */
	if (GetButtonDown(Keyboard::DOWN)) {
		return true;
	}

	/*! 入力マスクのリセット */
	isInput = (isInput & 0b11111101);

	return false;
}

/*!
	@brief	決定ボタン
*/
bool InputManager::GetDecideDown()
{
	if (pGamePad->GetButtonDown(Button::A) ||
		Keyboard::GetButtonDown(Keyboard::SPACE)||
		Keyboard::GetButtonDown(Keyboard::ENTER)) {
		return true;
	}


	return false;
}

/*!
	@brief	BACKボタン
*/
bool InputManager::GetBackDown()
{
	return pGamePad->GetButtonDown(Button::BACK);
}
