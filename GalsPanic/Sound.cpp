#include "Sound.h"

void Sound::Open(HWND _hWnd, const WCHAR* path)
{
	hWnd = _hWnd;

	mciOpen.lpstrElementName = path;
	mciOpen.lpstrDeviceType = TEXT("mpegvideo");
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
		(DWORD)(LPVOID)&mciOpen);
	dwID = mciOpen.wDeviceID;
}

void Sound::Play(DWORD volume, bool repeat)
{

	Volume(volume);
	mciSendCommand(dwID, MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)&mciSeek);

	if(repeat)
		mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&mciOpen);
	else
		mciSendCommand(dwID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&mciOpen);
}

void Sound::Pause()
{
	mciSendCommand(dwID, MCI_PAUSE, MCI_NOTIFY, (DWORD)(LPVOID)&mciOpen);
}

void Sound::Volume(DWORD volume)
{
	if (volume > 300) volume = 300;
	if (volume < 0) volume = 0;

	mciAudio.dwCallback = 0;
	mciAudio.dwItem = MCI_DGV_SETAUDIO_VOLUME;
	mciAudio.dwValue = volume;
	mciAudio.dwOver = 0;
	mciAudio.lpstrAlgorithm = NULL;
	mciAudio.lpstrQuality = NULL;

	mciSendCommand(dwID, MCI_SETAUDIO, MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM,
		(DWORD)(LPVOID)&mciAudio);
}

void Sound::Stop()
{
	mciSendCommand(dwID, MCI_CLOSE, MCI_NOTIFY, (DWORD)(LPVOID)&mciOpen);
}
