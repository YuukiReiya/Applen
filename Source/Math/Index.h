/*!
	@file	Index.h
	@date	2018/04/09
	@author	”Ôê —G‹P
	@brief	ƒCƒ“ƒfƒbƒNƒX—p\‘¢‘Ì
*/
#pragma once

struct INDEX
{
	int x;
	int y;

	INDEX() :x(0), y(0) {}
	INDEX(int ix, int iy) :x(ix), y(iy) {}

	/**************/
	/* ƒIƒyƒŒ[ƒ^ */
	/**************/

	/*! ‘«‚µŽZ */
	INDEX operator +(INDEX const& val) {
		return INDEX(this->x + val.x, this->y + val.y);
	}

	/*! ˆø‚«ŽZ */
	INDEX operator -(INDEX const& val) {
		return INDEX(this->x - val.x, this->y - val.y);
	}

	/*! Š|‚¯ŽZ */
	INDEX operator *(int val) {
		return INDEX(this->x*val, this->y*val);
	}
	INDEX operator *(INDEX const& val) {
		return INDEX(this->x*val.x, this->y*val.y);
	}

	/*! Š„‚èŽZ */
	INDEX operator /(int val) {
		return INDEX(this->x / val, this->y / val);
	}
	INDEX operator /(INDEX const& val) {
		return INDEX(this->x / val.x, this->y / val.y);
	}

	/*! ”»’è */
	bool operator ==(INDEX const& val) {
		return this->x == val.x&&this->y == val.y;
	}
	bool operator !=(INDEX const& val) {
		return this->x != val.x || this->y != val.y;
	}
};