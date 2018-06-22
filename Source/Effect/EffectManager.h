/*!
	@file	EffectManager.h
	@date	2018/06/22
	@author	番場 宥輝
	@brief	エフェクト管理クラス
	@def	シングルトン
*/
#pragma once
#if _DEBUG
#pragma comment(lib, "Effekseer.lib" )
#pragma comment(lib, "EffekseerRendererDX11.lib" )
#pragma comment(lib, "EffekseerSoundXAudio2.lib" )
#else
#pragma comment(lib, "Effekseer.lib" )
#pragma comment(lib, "EffekseerRendererDX11.lib" )
#pragma comment(lib, "EffekseerSoundXAudio2.lib" )
#endif

#include <stdio.h>
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
#include <EffekseerSoundXAudio2.h>




class EffectManager
{
public:
	static EffectManager & GetInstance() {
		static EffectManager instance;
		return instance;
	}


	void Init();


	static const int MAX_RENDER_SPRITE;
	~EffectManager();
private:
	EffectManager();


	Effekseer::Manager*				pInstance;
	EffekseerRenderer::Renderer*	pRender;
	EffekseerSound::Sound*			pSound;
	Effekseer::Effect*				pEffect;
	Effekseer::Handle				handle;
	//Effekseer::Vector3D				pos;
};

