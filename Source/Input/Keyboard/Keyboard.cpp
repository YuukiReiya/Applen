/*
	@file	Keyboard.cpp
	@brief	Windowsキーボード入力
	@date	2017/08/09
	@detail	キーコード確認用リンク:http://www.azaelia.net/factory/vk.html
	@author	番場 宥輝
*/
#include "Keyboard.h"

/*! 名前空間 */
namespace Keyboard {

	/*! キーの数 */
	const int keyNum = 256;

	/*
		@struct	Keyboard
		@brief	キーの状態を保存する構造体
	*/
	struct Keyboard
	{
		BYTE keys[keyNum];
	};

	Keyboard now,	/*!< 入力状況 */
			 old;	/*!< 1フレーム前の入力状況 */

	/*
		@brief	更新処理
	*/
	void Update()
	{
		old = now;						/*!< oldの更新 */

		GetKeyboardState(now.keys);		/*!< キーの取得 */
	}


	/*!< 押している間 */
	bool GetButton(BYTE key)
	{
		return now.keys[key] & 0x80;
	}

	/*!< 押した瞬間 */
	bool GetButtonDown(BYTE key)
	{
		return (now.keys[key] ^ old.keys[key]) & now.keys[key];
	}

	/*!< 離された瞬間 */
	bool GetButtonUp(BYTE key)
	{
		return (now.keys[key] ^ old.keys[key]) & old.keys[key];
	}
}