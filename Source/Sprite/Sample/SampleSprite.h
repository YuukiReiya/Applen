/**
* @file		TitleSprite.h
* @brief	�^�C�g���̃X�v���C�g
* @author	�� ����
* @date�@	2018/02/20
*/
#pragma once
#include "../Sprite.h"

class SampleSprite
{
private:
	umap_str_sprite tMSprite;
	uint32_t ePhase;

public:
	SampleSprite() {};
	~SampleSprite() {};

	void Init();
	void Update(const uint32_t phase);
	void Render();
	void ReLoad();
};