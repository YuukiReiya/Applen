/*
	@file	Wave.h
	@date	2018/02/11
	@author	�ԏ� �G�P
	@brief	Wave�T�E���h
*/
#include "Wave.h"
#include "../SoundManager/SoundManager.h"


/*!
	@brief	�R���X�g���N�^
*/
Wave::Wave()
{
	SecureZeroMemory(this, sizeof(this));
}

/*!
	@brief	�f�X�g���N�^
*/
Wave::~Wave()
{
	Release();
}

/*!
	@brief	Wave�t�@�C���ǂݍ���
*/
bool Wave::Load(std::string filePath)
{
	HMMIO hMmio = NULL;				/*!< Windows�}���`���f�B�AAPI�n���h�� */
	DWORD waveSize = 0;				/*!< Wave�f�[�^�T�C�Y */
	
	WAVEFORMATEX*	pFormat;		/*!< Wave�t�H�[�}�b�g */
	MMCKINFO		chunkInfo;		/*!< �`�����N��� */
	MMCKINFO		riffChunkInfo;	/*!< �ŏ㕔�`�����N */
	PCMWAVEFORMAT	pcmFormat;		/*!< PCM�t�H�[�}�b�g */

	/*! Wave�t�@�C�����̃w�b�_�[���ǂݍ��� */

	std::wstring tmp;
	/*! �����R�[�h�ϊ��X�R�[�v */
	{
		std::wstring ret;
		size_t length = 0;
		ret.resize(MAX_PATH - 1);
		mbstowcs_s(&length, &ret.front(), ret.size(), filePath.c_str(), _TRUNCATE);
		tmp = ret;
	}
	const auto path = const_cast<LPTSTR>(tmp.c_str());
	hMmio = mmioOpenW(path, NULL, MMIO_ALLOCBUF | MMIO_READ);

	/*! Wave�t�@�C���̓ǂݍ��ݎ��s */
	if (hMmio == NULL) {
		return false;
	}

	/*! �t�@�C���|�C���^��RIFF�`�����N�̐擪�ɃZ�b�g */
	mmioDescend(hMmio, &riffChunkInfo, NULL, 0);

	/*! �t�@�C���|�C���^��'f' 'm' 't' ' ' �`�����N�ɃZ�b�g���� */
	chunkInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmioDescend(hMmio, &chunkInfo, &riffChunkInfo, MMIO_FINDCHUNK);

	/*! �t�H�[�}�b�g�ǂݍ��� */
	mmioRead(hMmio, (HPSTR)&pcmFormat, sizeof(pcmFormat));
	pFormat = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX)];
	memcpy(pFormat, &pcmFormat, sizeof(pcmFormat));
	pFormat->cbSize = 0;
	mmioAscend(hMmio, &chunkInfo, 0);

	/*! ���f�[�^�ǂݍ��� */
	chunkInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmioDescend(hMmio, &chunkInfo, &riffChunkInfo, MMIO_FINDCHUNK);
	waveSize = chunkInfo.cksize;
	m_pWaveBuffer = new BYTE[waveSize];
	DWORD offset = chunkInfo.dwDataOffset;
	mmioRead(hMmio, (char*)m_pWaveBuffer, waveSize);

	/*! �\�[�X�{�C�X�Ƀf�[�^�l�ߍ��� */
	XAUDIO2_SEND_DESCRIPTOR sendDescriptor;
	sendDescriptor.Flags = XAUDIO2_SEND_USEFILTER;
	sendDescriptor.pOutputVoice = SoundManager::GetMasterVoice();
	const XAUDIO2_VOICE_SENDS sendList = { 1,&sendDescriptor };

	/*! �\�[�X�{�C�X�쐬 */
	HRESULT hr = SoundManager::GetXAudio2()->CreateSourceVoice(&m_pSourceVoice, pFormat, 0, 2.0f, &m_Callback, &sendList);

	if (pFormat != nullptr) {
		delete pFormat;
	}

	if (FAILED(hr)) {
		return false;
	}
	m_dwWaveSize = waveSize;

	return true;
}

/*!
	@brief	���
*/
void Wave::Release()
{
	m_pSourceVoice->DestroyVoice();
	if (m_pWaveBuffer != nullptr) { delete m_pWaveBuffer; }
}

/*!
	@brief	�Đ�
*/
void Wave::Play(bool isLoop)
{
	/*! �T�u�~�b�g */
	XAUDIO2_BUFFER buffer = { 0 };
	SecureZeroMemory(&buffer, sizeof(buffer));
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = m_dwWaveSize;
	buffer.pAudioData = m_pWaveBuffer;
	buffer.PlayBegin = 0;	/*!< �Đ������o�b�t�@�̍ŏ��̃T���v�� */
	buffer.PlayLength = 0;	/*!< �T�E���h�̃o�b�t�@�S�� */

	if (isLoop) {

		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;

	}
	else {
		buffer.LoopBegin = XAUDIO2_NO_LOOP_REGION;
		buffer.LoopCount = 0;
	}
	HRESULT hr = m_pSourceVoice->SubmitSourceBuffer(&buffer);
	if (FAILED(hr)) {
		return;
	}
	m_pSourceVoice->Start(0, XAUDIO2_COMMIT_NOW);
}

/*!
	@brief	��~
*/
void Wave::Stop()
{
	m_pSourceVoice->Stop(0, XAUDIO2_COMMIT_NOW);
	m_pSourceVoice->FlushSourceBuffers();
}

/*!
	@brief	�ꎞ��~
*/
void Wave::Pause()
{
	m_pSourceVoice->Stop(0, XAUDIO2_COMMIT_NOW);
}

/*!
	@brief	���ʕύX
	@detail	�͈� 0�`1
*/
void Wave::SetVolume(float vol)
{
	m_pSourceVoice->SetVolume(vol);
}

void Wave::XAudio2VoiceCallback::OnBufferEnd(void * call)
{
	/*! ���\�b�h���o�^����ĂȂ���Ώ����𔲂��� */
	if (call == NULL) { return; }

	/*! �֐��^�ɃL���X�g */
	FPVOID fp = static_cast<FPVOID>(call);

	/*! ���\�b�h�Ăяo�� */
	fp();
}

void Wave::XAudio2VoiceCallback::OnBufferStart(void * call)
{
	/*! ���\�b�h���o�^����ĂȂ���Ώ����𔲂��� */
	if (call == NULL) { return; }

	/*! �֐��^�ɃL���X�g */
	FPVOID fp = static_cast<FPVOID>(call);

	/*! ���\�b�h�Ăяo�� */
	fp();

}

void Wave::XAudio2VoiceCallback::OnLoopEnd(void * call)
{
	/*! ���\�b�h���o�^����ĂȂ���Ώ����𔲂��� */
	if (call == NULL) { return; }

	/*! �֐��^�ɃL���X�g */
	FPVOID fp = static_cast<FPVOID>(call);

	/*! ���\�b�h�Ăяo�� */
	fp();

}

void Wave::XAudio2VoiceCallback::OnStreamEnd()
{
}

void Wave::XAudio2VoiceCallback::OnVoiceError(void * call, HRESULT)
{
	/*! ���\�b�h���o�^����ĂȂ���Ώ����𔲂��� */
	if (call == NULL) { return; }

	/*! �֐��^�ɃL���X�g */
	FPVOID fp = static_cast<FPVOID>(call);

	/*! ���\�b�h�Ăяo�� */
	fp();

}

void Wave::XAudio2VoiceCallback::OnVoiceProcessingPassEnd()
{
}

void Wave::XAudio2VoiceCallback::OnVoiceProcessingPassStart(UINT32)
{
}
