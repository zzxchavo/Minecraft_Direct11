#include "SoundClass.h"


SoundClass::SoundClass()
{
	m_DirectSound = NULL;
	m_primaryBuffer = NULL;
	m_secondaryBuffer1 = NULL;
}


SoundClass::~SoundClass()
{
}



HRESULT SoundClass::Initialize(HWND hwnd,char* fileName)
{
	HRESULT hr = S_OK;
	hr = InitializeDirectSound(hwnd);
	if (FAILED(hr))
	{
		return hr;
	}
	if (!LoadWaveFile(fileName, &m_secondaryBuffer1))
	{
		return E_FAIL;
	}
	hr = PlayWaveFile();
	if (FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

void SoundClass::ShutDown()
{
	ShutdownWaveFile(&m_secondaryBuffer1);
	ShutdownDirectSound();
	return ;
}

bool SoundClass::InitializeDirectSound(HWND hwnd)
{
	HRESULT hr;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;
	hr = DirectSoundCreate8(NULL,&m_DirectSound,NULL);
	if (FAILED(hr))
	{
		return hr;
	}
	
	hr = m_DirectSound->SetCooperativeLevel(hwnd,DSSCL_PRIORITY);
	if (FAILED(hr))
	{
		return hr;
	}

	ZeroMemory(&bufferDesc,sizeof(bufferDesc));
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.guid3DAlgorithm = GUID_NULL;
	hr = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer,NULL);
	if (FAILED(hr))
	{
		return hr;
	}

	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8)*waveFormat.nChannels;
	waveFormat.nBlockAlign = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;
	hr = m_primaryBuffer->SetFormat(&waveFormat);
	if (FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

void SoundClass::ShutdownDirectSound()
{
	if (m_primaryBuffer)
	{
		m_primaryBuffer->Release();
		m_primaryBuffer = NULL;
	}
	if (m_DirectSound)
	{
		m_DirectSound->Release();
		m_DirectSound = NULL;
	}
}


bool SoundClass::LoadWaveFile(char* fileName, IDirectSoundBuffer8** secondaryBuffer)
{
	int error;
	FILE * filePtr = NULL;
	unsigned int count;
	WaveHeaderType waveHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	HRESULT hr;
	IDirectSoundBuffer* temp;
	unsigned char* waveData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;
	error = fopen_s(&filePtr, fileName, "rb");
	if (error != 0)
	{
		return false;
	}
	count = fread(&waveHeader, sizeof(waveHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	if (waveHeader.chunkId[0] != 'R' ||
		waveHeader.chunkId[1] != 'I' ||
		waveHeader.chunkId[2] != 'F' ||
		waveHeader.chunkId[3] != 'F')
	{
		return false;
	}
	if (waveHeader.format[0] != 'W' ||
		waveHeader.format[1] != 'A' || 
		waveHeader.format[2] != 'V' ||
		waveHeader.format[3] != 'E');
	{
		return false;
	}
	if (waveHeader.subChunkId[0] != 'f' ||
		waveHeader.subChunkId[1] != 'm' || 
		waveHeader.subChunkId[2] != 't' ||
		waveHeader.subChunkId[3] != ' ')
	{
		return false;
	}
	if (waveHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		return false;
	}
	if (waveHeader.numChannels != 2)
	{
		return false;
	}
	if (waveHeader.sampleRate != 44100)
	{
		return false;
	}
	if (waveHeader.bitsPerSample != 16)
	{
		return false;
	}
	if (waveHeader.dataChunkId[0] != 'd' ||
		waveHeader.dataChunkId[1] != 'a' ||
		waveHeader.dataChunkId[2] != 't' ||
		waveHeader.dataChunkId[3] != 'a')
	{
		return false;
	}

	ZeroMemory(&waveFormat, sizeof(waveFormat));
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	hr = m_DirectSound->CreateSoundBuffer(&bufferDesc, &temp, NULL);
	if (FAILED(hr))
	{
		return false;
	}
	hr = temp->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	if (temp)
	{
		temp->Release();
		temp = NULL;
	}
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);
	waveData = new unsigned char[waveHeader.dataSize];
	if (!waveData)
	{
		return false;
	}
	
	count = fread(waveData, 1, waveHeader.dataSize, filePtr);
	if (count != waveHeader.dataSize)
	{
		return false;
	}
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}
	hr = (*secondaryBuffer)->Lock(0, waveHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if (FAILED(hr))
	{
		return false;
	}

	memcpy(bufferPtr, waveData, waveHeader.dataSize);
	hr = (*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if (FAILED(hr))
	{
		return false;
	}
	if (waveData)
	{
		delete []  waveData;
		waveData = NULL;
	}
	return true;
}

void SoundClass::ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer)
{
	if (*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = NULL;
	}
}

bool SoundClass::PlayWaveFile()
{
	HRESULT hr;
	hr = m_secondaryBuffer1->SetCurrentPosition(0);
	if (FAILED(hr))
	{
		return false;
	}

	hr = m_secondaryBuffer1->SetVolume(DSBVOLUME_MAX);
	if (FAILED(hr))
	{
		return false;
	}

	hr = m_secondaryBuffer1->Play(0, 0, 0);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}