#pragma once
#include "Gutil.h"
#include <mmsystem.h>
#include <mciapi.h>
#include <Digitalv.h>
#pragma comment(lib, "winmm.lib")

class Sound
{
	int						dwID;
	HWND					hWnd;
	MCI_OPEN_PARMS			mciOpen;
	MCI_SEEK_PARMS			mciSeek;
	MCI_STATUS_PARMS		mciStatus;

	MCI_DGV_SETAUDIO_PARMS	mciAudio;
	

public:
	void Open(HWND _hWnd, const WCHAR* path);
	void Play(DWORD volume, bool repeat);
	void Play(DWORD volume, bool repeat, bool stop);
	void Volume(DWORD volume);
	void PlayString(const WCHAR* path);
	void Stop();
};

class SoundPlayer
{
	
	Sound bgm;
	Sound OK;
	Sound H;
	Sound Shot;
	Sound H_Shot;
	Sound Eri;
	Sound EnemyDeath;
	Sound Bomb;
	Sound EriDeath;
	Sound Thankyou;
	Sound Once;

	HWND hWnd;
public:
	~SoundPlayer();
	void Init();
	void BGMPlay(int type);
	void ItemPlay(int type);
	void EffectPlay(int type);
	void DeathPlay(int type);
	void ButtonPlay(int type);
	void MissionPlay(int type);
};

