#pragma once
#include<Windows.h>
#include<mmsystem.h>
#include<dsound.h>
#include<stdio.h>



class SoundClass
{
public:
	SoundClass();
	~SoundClass();

	HRESULT Initialize(HWND,char*);
	void ShutDown();


	bool PlayWaveFile();

private:
	bool InitializeDirectSound(HWND);
	void ShutdownDirectSound();

	bool LoadWaveFile(char *, IDirectSoundBuffer8**);
	void ShutdownWaveFile(IDirectSoundBuffer8**);


	IDirectSound8*			m_DirectSound;
	IDirectSoundBuffer*		m_primaryBuffer;
	IDirectSoundBuffer8*	m_secondaryBuffer1;


	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

};

