/*
	@file	GamePad.h
	@brief	ゲームパッド入力
	@date	2018/04/24
	@detail	XInputを用いた入力
			XInput参考:		http://www.geocities.jp/gameprogrammingunit/xinput/index.html
			列挙体の注意:	http://d.hatena.ne.jp/mclh46/20100627/1277603928
	@author	番場 宥輝
*/#pragma once
#include <windows.h>
#include <Xinput.h>
#include <DirectXMath.h>

#pragma comment(lib, "XInput.lib")

/*!
	@brief	名前空間
*/
namespace KeyCode {
	/*! 列挙体キーコード */
	enum Button { A, B, X, Y, START, BACK, LB, RB, L_STICK, R_STICK, };	/*!< ボタン */
	enum Trigger { LT, RT };											/*!< トリガー */
	enum JoyStick { LEFT, RIGHT };										/*!< ジョイスティック */
	enum Arrow { VERTICAL, HORIZONTAL };								/*!< V:縦 H:横 */
}

/*!
	@brief	ゲームパッドクラス
	@detail	XBOXコントローラーの入力制御クラス
*/
class GamePad {
public:
	/*! コントローラー番号 */
	enum Index { One, Two, Three, Four, };

	GamePad();
	GamePad(Index&& index);
	~GamePad();


	/*! 関数 */
	void Update();
	bool GetButton(KeyCode::Button button);
	bool GetButtonDown(KeyCode::Button button);
	bool GetButtonUp(KeyCode::Button button);
	int GetArrow(KeyCode::Arrow arrow);
	int GetArrowDown(KeyCode::Arrow arrow);
	int GetArrowUp(KeyCode::Arrow arrow);
	DirectX::XMFLOAT3 GetJoyStick(KeyCode::JoyStick joyStick);
	BYTE GetTrigger(KeyCode::Trigger trigger);
	bool GetBTrigger(KeyCode::Trigger trigger);
	void SetVibration(int leftPower, int rightPower = -1);
private:
	float InputNormalize(float input);
	float InputRound(float input);
	int VibrationConvertPower(int vibration);
	int ConvertKeyButtonCode(KeyCode::Button&& button);
	DirectX::XMINT2 ConvertKeyArrowCode(KeyCode::Arrow&& arrow);

	/*! 定数 */
	static const int c_JoyStickInputMin;
	static const int c_JoyStickInputMax;
	static const int c_JoyStickInputPrecision;
	static const int c_VibrationRange;
	static const float c_JoyStickInputDead;

	/*! パラメーター */
	Index index;			/*!< コントローラー番号 */
	XINPUT_STATE now,old;	/*!< 入力バッファ */

};