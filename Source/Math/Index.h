/*!
	@file	Index.h
	@date	2018/04/09
	@author	番場 宥輝
	@brief	インデックス用構造体
*/
#pragma once

struct INDEX
{
	int x;
	int y;

	INDEX() :x(0), y(0) {}
	INDEX(int ix, int iy) :x(ix), y(iy) {}

	/**************/
	/* オペレータ */
	/**************/

	/*! 足し算 */
	INDEX operator +(INDEX const& val) {
		return INDEX(this->x + val.x, this->y + val.y);
	}

	/*! 引き算 */
	INDEX operator -(INDEX const& val) {
		return INDEX(this->x - val.x, this->y - val.y);
	}

	/*! 掛け算 */
	INDEX operator *(int val) {
		return INDEX(this->x*val, this->y*val);
	}
	INDEX operator *(INDEX const& val) {
		return INDEX(this->x*val.x, this->y*val.y);
	}

	/*! 割り算 */
	INDEX operator /(int val) {
		return INDEX(this->x / val, this->y / val);
	}
	INDEX operator /(INDEX const& val) {
		return INDEX(this->x / val.x, this->y / val.y);
	}

	/*! 判定 */
	bool operator ==(INDEX const& val) {
		return this->x == val.x&&this->y == val.y;
	}
	bool operator !=(INDEX const& val) {
		return this->x != val.x || this->y != val.y;
	}
};