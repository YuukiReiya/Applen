/*
	@file	Wave.h
	@date	2018/02/11
	@author	番場 宥輝
	@brief	Waveサウンド
*/
#pragma once
#include <string>
#include <XAudio2.h>
#include <functional>



class Wave
{
	/*! 構造体定義 */
private:
	/*! コールバック用別名定義 */
	typedef void(*FPVOID)();

	struct XAudio2VoiceCallback
		:public IXAudio2VoiceCallback
	{
		XAudio2VoiceCallback(){
			m_fpWaveEnd		= NULL;
			m_fpWaveStart	= NULL;
			m_fpWaveLoopEnd = NULL;
			m_fpWaveError	= NULL;
		}
		void* m_fpWaveEnd;
		void* m_fpWaveStart;
		void* m_fpWaveLoopEnd;
		void* m_fpWaveError;
		void OnBufferEnd(void* call)override;
		void OnBufferStart(void* call)override;
		void OnLoopEnd(void* call)override;
		void OnStreamEnd()override;
		void OnVoiceError(void* call, HRESULT)override;
		void OnVoiceProcessingPassEnd()override;
		void OnVoiceProcessingPassStart(UINT32)override;
	};
public:
	Wave();
	~Wave();

	bool Load(std::string filePath);
	void Release();
	void Play(bool isLoop = false);
	void Stop();
	void Pause();
	void SetVolume(float vol);
	void SetWaveEndFunc		(void*call) { m_Callback.m_fpWaveEnd	  = call; }
	void SetWaveStartFunc	(void*call)	{ m_Callback.m_fpWaveStart	  = call; }
	void SetWaveLoopEndFunc	(void*call)	{ m_Callback.m_fpWaveLoopEnd  = call; }
	void SetWaveErrorFunc	(void*call)	{ m_Callback.m_fpWaveError	  = call; }
private:
	IXAudio2SourceVoice* m_pSourceVoice;
	BYTE* m_pWaveBuffer;
	DWORD m_dwWaveSize;
	XAudio2VoiceCallback m_Callback;
};

