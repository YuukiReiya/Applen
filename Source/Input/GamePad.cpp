/*
	@file	Keyboard.h
	@brief	Windowsキーボード入力
	@date	2018/04/24
	@detail	キーコード確認用リンク:http://www.azaelia.net/factory/vk.html
	@author	番場 宥輝
*/
#include "GamePad.h"
#include <math.h>

/*!
	@def	定数宣言
*/
const int GamePad::c_JoyStickInputMin		= -32768;
const int GamePad::c_JoyStickInputMax		=  32767;
const int GamePad::c_JoyStickInputPrecision =  10;		/*!< 精度 ex)10:少数第一位、100:少数第二位 */
const int GamePad::c_VibrationRange			= 65535;	/*!< バイブレーションの値域の範囲 0～65535 */
const float GamePad::c_JoyStickInputDead	=  0.0f;	/*!< この値以下の値は 0.0f として扱われる ※ジョイスティックのみ対応 */

/*!
	@brief	名前空間
*/
using namespace KeyCode;

/*!
	@brief	コンストラクタ
*/
GamePad::GamePad()
{
	SecureZeroMemory(this, sizeof(this));
	index = Index::One;
}

/*!
	@brief		引数付きコンストラクタ
	@param[in]	設定するコントローラー番号
*/
GamePad::GamePad(Index && index)
{
	SecureZeroMemory(this, sizeof(this));
	this->index = index;
}

/*!
	@brief	デストラクタ
*/
GamePad::~GamePad()
{
	SetVibration(0);
	//XInputEnable(false);/*!< XInputを無効にする(全てのコントローラーに依存) */
}

/*!
	@brief	入力バッファの更新
*/
#include <string>
void GamePad::Update()
{
	old = now;
	if (GetJoyStick(JoyStick::LEFT).y != 0) {


		std::string t = "x=" + std::to_string(GetJoyStick(JoyStick::LEFT).x) + "\ny=" + std::to_string(GetJoyStick(JoyStick::LEFT).y);

	//MessageBoxA(0, t.c_str(), 0, MB_OK);
	}

	if (XInputGetState(index, &now) == ERROR_SUCCESS) {}
}

/*!
	@brief	ボタンを押してる間
*/
bool GamePad::GetButton(Button button)
{
	return now.Gamepad.wButtons&ConvertKeyButtonCode(Button(button));
}

/*!
	@brief	十字キーを押してる間
*/
int GamePad::GetArrow(KeyCode::Arrow arrow)
{
	auto input = ConvertKeyArrowCode(Arrow(arrow));

	/*! 正の値 */
	if ((now.Gamepad.wButtons&input.x)==1) {
		return 1;
	}
	/*! 負の値 */
	if ((now.Gamepad.wButtons&input.y)==1) {
		return -1;
	}

	return 0;
}

/*!
	@brief	ボタンを押した瞬間
*/
bool GamePad::GetButtonDown(KeyCode::Button button)
{
	return ((now.Gamepad.wButtons&ConvertKeyButtonCode(Button(button))) 
		^
		(old.Gamepad.wButtons&ConvertKeyButtonCode(Button(button))))
		&
		(now.Gamepad.wButtons&ConvertKeyButtonCode(Button(button)));
}

/*!
	@brief	十字キーを押した瞬間
*/
int GamePad::GetArrowDown(KeyCode::Arrow arrow)
{
	auto input = ConvertKeyArrowCode(Arrow(arrow));

	/*! 正の値 */
	if ((now.Gamepad.wButtons&input.x)
		^
		(old.Gamepad.wButtons&input.x)
		&
		(now.Gamepad.wButtons&input.x)
		) {
		return 1;
	}

	/*! 負の値 */
	if ((now.Gamepad.wButtons&input.y)
		^
		(old.Gamepad.wButtons&input.y)
		&
		(now.Gamepad.wButtons&input.y)
		) {
		return -1;
	}

	return 0;
}

/*!
	@brief	ボタンが離された瞬間
*/
bool GamePad::GetButtonUp(KeyCode::Button button)
{
	return ((now.Gamepad.wButtons&ConvertKeyButtonCode(Button(button)))
		^
		(old.Gamepad.wButtons&ConvertKeyButtonCode(Button(button))))
		&
		(old.Gamepad.wButtons&ConvertKeyButtonCode(Button(button)));
}

/*!
	@brief	十字キーが離された瞬間
*/
int GamePad::GetArrowUp(KeyCode::Arrow arrow)
{
	auto input = ConvertKeyArrowCode(Arrow(arrow));

	/*! 正の値 */
	if ((now.Gamepad.wButtons&input.x)
		^
		(old.Gamepad.wButtons&input.x)
		&
		(old.Gamepad.wButtons&input.x)
		) {
		return 1;
	}

	/*! 負の値 */
	if ((now.Gamepad.wButtons&input.y)
		^
		(old.Gamepad.wButtons&input.y)
		&
		(old.Gamepad.wButtons&input.y)
		) {
		return -1;
	}

	return 0;
}

/*!
	@brief	ジョイスティックの入力取得
	@detail	x:横軸、y:縦軸、z:押し込み(0か1)
*/
DirectX::XMFLOAT3 GamePad::GetJoyStick(KeyCode::JoyStick joyStick)
{
	DirectX::XMFLOAT3 input = { 0,0,0 };

	switch (joyStick)
	{
	case KeyCode::JoyStick::LEFT:
		input.x = now.Gamepad.sThumbLX;
		input.y = now.Gamepad.sThumbLY;
		/*! 左スティックの押し込み */
		input.z = now.Gamepad.wButtons & 0x0040 ? 1.0f : 0.0f;
		break;
	case KeyCode::JoyStick::RIGHT:
		input.x = now.Gamepad.sThumbRX;
		input.y = now.Gamepad.sThumbRY;
		/*! 右スティックの押し込み */
		input.z = now.Gamepad.wButtons & 0x0080 ? 1.0f : 0.0f;
		break;
	default:
		input = DirectX::XMFLOAT3(0, 0, 0);
		break;
	}

	/*! 値の正規化(-1～1) */
	input.x = InputNormalize(input.x);
	input.y = InputNormalize(input.y);

	/*! 値を丸める */
	input.x = InputRound(input.x);
	input.y = InputRound(input.y);

	return input;
}

/*!
	@brief	トリガーの入力取得
*/
BYTE GamePad::GetTrigger(KeyCode::Trigger trigger)
{
	BYTE input = 0;
	
	switch (trigger)
	{
	case KeyCode::LT:
		input = now.Gamepad.bLeftTrigger;
		break;
	case KeyCode::RT:
		input = now.Gamepad.bRightTrigger;
		break;
	default:
		break;
	}

	return input;
}

/*!
	@brief	トリガーの入力をboolで判定
*/
bool GamePad::GetBTrigger(KeyCode::Trigger trigger)
{
	return GetTrigger(trigger) > 0;
}

/*!
	@brief	バイブレーションの設定
*/
void GamePad::SetVibration(int leftPower, int rightPower)
{
	/*! 左右で出力が同じなら */
	if (rightPower == -1) {
		rightPower = leftPower;
	}
	XINPUT_VIBRATION vibration;
	SecureZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = VibrationConvertPower(leftPower);
	vibration.wRightMotorSpeed = VibrationConvertPower(rightPower);
	XInputSetState(index, &vibration);
}

/*!
	@brief	入力値の正規化
	@detail	-1～1の範囲に値を正規化した値を返す
*/
float GamePad::InputNormalize(float input)
{
	input = (input - c_JoyStickInputMin) / (c_JoyStickInputMax - c_JoyStickInputMin)*(1 - (-1)) + (-1);
	return input;
}

/*!
	@brief	入力値を丸める
	@detail	設定した精度で数値を丸める
*/
float GamePad::InputRound(float input)
{
	int tmp = static_cast<int>(input) * c_JoyStickInputPrecision;
	input = static_cast<float>(tmp) / static_cast<float>(c_JoyStickInputPrecision);

	/*! 設定した入力値以下なら 入力値を0にする */
	if (fabs(input) < c_JoyStickInputDead) {
		input = 0.0f;
	}

	return input;
}

/*!
	@brief	正規化した値をバイブレーション値に変換
	@detail	0～100の範囲を受け取り
*/
int GamePad::VibrationConvertPower(int vibration)
{
	/*! 範囲外補正 */
	{
		if (vibration < 0) { vibration = 0; }
		else if (100 < vibration) { vibration = 100; }
	}
	vibration *= c_VibrationRange / 100;
	return vibration;
}

/*!
	@brief	ボタンのキーコード変換
	@detail	ボタンの列挙体からXInputの判定用16進数へ変換
*/
int GamePad::ConvertKeyButtonCode(Button && button)
{
	switch (button)
	{
	case Button::A:			return 0x1000;	/*!< Aボタン マクロ【 XINPUT_GAMEPAD_A】 */
	case Button::B:			return 0x2000;	/*!< Bボタン マクロ【 XINPUT_GAMEPAD_B】 */
	case Button::X:			return 0x4000;	/*!< Xボタン マクロ【 XINPUT_GAMEPAD_X】 */
	case Button::Y:			return 0x8000;	/*!< Yボタン マクロ【 XINPUT_GAMEPAD_Y】 */
	case Button::START:		return 0x0010;	/*!< STARTボタン マクロ【 XINPUT_GAMEPAD_START】 */
	case Button::BACK:		return 0x0020;	/*!< BACKボタン マクロ【 XINPUT_GAMEPAD_BACK】 */
	case Button::LB:		return 0x0100;	/*!< LBボタン マクロ【 XINPUT_GAMEPAD_LEFT_SHOULDER】 */
	case Button::RB:		return 0x0200;	/*!< RBボタン マクロ【 XINPUT_GAMEPAD_RIGHT_SHOULDER】 */
	case Button::L_STICK:	return 0x0040;	/*!< 左スティックの押し込み マクロ【 XINPUT_GAMEPAD_LEFT_THUMB】 */
	case Button::R_STICK:	return 0x0080;	/*!< 右スティックの押し込み マクロ【 XINPUT_GAMEPAD_RIGHT_THUMB】 */
	default:
		break;
	}
	return 0;
}

/*!
	@brief	十字キーのキーコード変換
	@detail	十字キーの列挙体からXInputの判定用16進数へ変換
*/
DirectX::XMINT2 GamePad::ConvertKeyArrowCode(KeyCode::Arrow && arrow)
{
	DirectX::XMINT2 code = { 0,0 };

	switch (arrow)
	{
	case KeyCode::VERTICAL:
		code.x = 0x0001;	/*!< 十字キーの上 */
		code.y = 0x0002;	/*!< 十字キーの下 */
		break;
	case KeyCode::HORIZONTAL:
		code.x = 0x0008;	/*!< 十字キーの右 */
		code.y = 0x0004;	/*!< 十字キーの左 */
		break;
	default:
		break;
	}
	return code;
}
