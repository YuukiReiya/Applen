/*!
	@file	FadeManager.h
	@date	2018/04/26
	@author	番場 宥輝
	@brief	フェード管理クラス
*/
#pragma once
#include "../Sprite/SpriteBase.h"
#include "functional"

class FadeManager
{
public:
	~FadeManager();

	static FadeManager&GetInstance() {
		static FadeManager instance;
		return instance;
	}

	void Initialize();
	void Update();
	void Render();


	bool IsFade() { return eState != State::NONE; }
	void StartFadeInOnly(int frame);
	void StartFadeOutOnly(int frame);
private:
	FadeManager();

	enum State
	:__int8{
		NONE,
		FADE_IN,
		FADE_OUT,
	}eState;

	unsigned int count;
	unsigned int fadeFrame;
	float addVal;
	SpriteBase* pSprite;
	TEXTURE_DATA* pTexData;
};

