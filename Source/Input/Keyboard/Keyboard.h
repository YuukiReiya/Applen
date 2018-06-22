/*
	@file	Keyboard.h
	@brief	Windowsキーボード入力
	@date	2017/08/09
	@detail	キーコード確認用リンク:http://www.azaelia.net/factory/vk.html
	@author	番場 宥輝
*/
#pragma once
#include<Windows.h>
#undef DELETE


/*! 名前空間 */
namespace Keyboard
{
	/*! 方向キー */
	const char UP    = 0x26;	/*!< ↑ */
	const char DOWN  = 0x28;	/*!< ↓ */
	const char LEFT  = 0x25;	/*!< ← */
	const char RIGHT = 0x27;	/*!< → */

	/*! 特殊キー */
	const char SPACE = 0x20;	/*! スペースキー		*/
	const char ENTER = 0x0D;	/*! エンターキー		*/
	const char SHIFT = 0x10;	/*! シフトキー			*/
	const char BACK  = 0x08;	/*! バックスペースキー	*/
	const char TAB   = 0x09;	/*! タブキー			*/
	const char DELETE= 0x2E;	/*!< デリートキー */


	/*! キーの更新タイミング(決まった場所で) */
	void Update();

	/*! 条件式 */
	bool GetButton(BYTE key);		/*!< 押してる間 */
	bool GetButtonDown(BYTE key);	/*!< 押した瞬間 */
	bool GetButtonUp(BYTE key);		/*!< 離した瞬間 */
}