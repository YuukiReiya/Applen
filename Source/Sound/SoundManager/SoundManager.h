/*
	@file	SoundManager.h
	@date	2018/03/02
	@author	番場 宥輝
	@brief	サウンド管理クラス
*/
#pragma once
#pragma comment(lib,"Xaudio2.lib")

#include <string>
#include <XAudio2.h>

class SoundManager
{
public:
	~SoundManager();
	static SoundManager& GetInstance() {
		static SoundManager instance;
		return instance;
	}
	bool Init(); 
	void Release();
	static IXAudio2* GetXAudio2() { return GetInstance().m_pXAudio2; }
	static IXAudio2MasteringVoice* GetMasterVoice() { return GetInstance().m_pMasteringVoice; }
private:
	SoundManager();
	IXAudio2*					m_pXAudio2;
	IXAudio2MasteringVoice*		m_pMasteringVoice;
};

