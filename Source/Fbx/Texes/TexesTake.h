/**
* @file		TexesTake.h
* @brief	�����e�N�X�`���̍쐬
* @author	���@����
* @date 	2018/02/20
*/
#pragma once
#include "../../Common.h"
#include <vector>
#include <string>

// �R���e�i�p
template <typename VR>
static inline void SAFE_RELEASE_CONTAINER(VR &p)
{
	int i;
	for (i = 0; i < p.size(); ++i)
	{
		p[i]->Release();
	}

	p.clear();
}

class TexesTake
{
private:
	std::vector<ID3D11ShaderResourceView*> vTex;
	HRESULT LoadUVTex(const char* path);

public:

	TexesTake(){};
	~TexesTake() { SAFE_RELEASE_CONTAINER(vTex); };
	

	HRESULT TexesCSVLoad(const char* path);
	ID3D11ShaderResourceView* GetTex(const uint32_t number) { return vTex[number]; };
};