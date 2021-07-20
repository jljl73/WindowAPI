#pragma once
#include "Gutil.h"
#include <mmsystem.h>
#include <mciapi.h>
#include <Digitalv.h>
#include <Windows.h>
#include <thread>
//#include <conio.h>
#pragma comment(lib, "winmm.lib")

class Sound
{
	int						dwID;
	HWND					hWnd;
	MCI_OPEN_PARMS			mciOpen;
	MCI_PLAY_PARMS			mciPlay;
	MCI_SEEK_PARMS			mciSeek;
	MCI_DGV_SETAUDIO_PARMS	mciAudio;
	MCI_STATUS_PARMS		mciStatus;
	DWORD					dwThlD;


public:
	~Sound()
	{
		Stop();
	}
	void Open(HWND hWnd, const WCHAR* path);
	void Play(DWORD, bool);
	void Pause();
	void Stop();
	void Volume(DWORD);
};