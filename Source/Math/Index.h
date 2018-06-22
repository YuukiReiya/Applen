/*!
	@file	Index.h
	@date	2018/04/09
	@author	�ԏ� �G�P
	@brief	�C���f�b�N�X�p�\����
*/
#pragma once

struct INDEX
{
	int x;
	int y;

	INDEX() :x(0), y(0) {}
	INDEX(int ix, int iy) :x(ix), y(iy) {}

	/**************/
	/* �I�y���[�^ */
	/**************/

	/*! �����Z */
	INDEX operator +(INDEX const& val) {
		return INDEX(this->x + val.x, this->y + val.y);
	}

	/*! �����Z */
	INDEX operator -(INDEX const& val) {
		return INDEX(this->x - val.x, this->y - val.y);
	}

	/*! �|���Z */
	INDEX operator *(int val) {
		return INDEX(this->x*val, this->y*val);
	}
	INDEX operator *(INDEX const& val) {
		return INDEX(this->x*val.x, this->y*val.y);
	}

	/*! ����Z */
	INDEX operator /(int val) {
		return INDEX(this->x / val, this->y / val);
	}
	INDEX operator /(INDEX const& val) {
		return INDEX(this->x / val.x, this->y / val.y);
	}

	/*! ���� */
	bool operator ==(INDEX const& val) {
		return this->x == val.x&&this->y == val.y;
	}
	bool operator !=(INDEX const& val) {
		return this->x != val.x || this->y != val.y;
	}
};