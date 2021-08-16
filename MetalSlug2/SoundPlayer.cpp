#include "SoundPlayer.h"
#include "GameManager.h"

void Sound::Open(HWND _hWnd, const WCHAR * path)
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

	if (repeat)
		mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&mciOpen);
	else
		mciSendCommand(dwID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&mciOpen);
}

void Sound::Play(DWORD volume, bool repeat, bool stop)
{
	Volume(volume);

	mciStatus.dwItem = MCI_STATUS_LENGTH;
	mciSendCommand(dwID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&mciStatus);
	int length = mciStatus.dwReturn;
	mciStatus.dwItem = MCI_STATUS_POSITION;
	mciSendCommand(dwID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&mciStatus);

	if(length == mciStatus.dwReturn)
		mciSendCommand(dwID, MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)&mciSeek);


	if (repeat)
		mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&mciOpen);
	else
		mciSendCommand(dwID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&mciOpen);
}

void Sound::Volume(DWORD volume)
{
	if (volume > 500) volume = 500;
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

void Sound::PlayString(const WCHAR * path)
{
	mciSendString(path, NULL, 0, NULL);
}

void Sound::Stop()
{
	if(dwID > 0)
		mciSendCommand(dwID, MCI_CLOSE, 0, NULL);
}


SoundPlayer::~SoundPlayer()
{
	Shot.Stop();
	OK.Stop();
	H.Stop();
	EnemyDeath.Stop();
	bgm.Stop();
}

void SoundPlayer::Init()
{
	hWnd = GameManager::instance()->hWnd;
	Shot.Open(hWnd, _T("data/Sound/effect/shot.mp3"));
	H_Shot.Open(hWnd, _T("data/Sound/effect/H_shot.mp3"));
	OK.Open(hWnd, _T("data/Sound/effect/OK.mp3"));
	H.Open(hWnd, _T("data/Sound/effect/H.mp3"));
	Eri.Open(hWnd, _T("data/Sound/effect/Eri.mp3"));
	EnemyDeath.Open(hWnd, _T("data/Sound/Enemy_Death.mp3"));
	EriDeath.Open(hWnd, _T("data/Sound/Death.mp3"));
	Bomb.Open(hWnd, _T("data/Sound/effect/Bomb.mp3"));
	Thankyou.Open(hWnd, _T("data/Sound/effect/Thankyou.mp3"));
}

void SoundPlayer::BGMPlay(int type)
{
	bgm.Stop();
	switch (type)
	{
	case 0:
		bgm.Open(hWnd, _T("data/Sound/bgm/Judgement.mp3"));
		break;
	case 1:
		bgm.Open(hWnd, _T("data/Sound/bgm/Title.mp3"));
		break;
	case 2:
		bgm.Open(hWnd, _T("data/Sound/bgm/Select.mp3"));
		break;
	case 3:
		bgm.Open(hWnd, _T("data/Sound/bgm/Boss.mp3"));
		break;
	case 4:
		bgm.Open(hWnd, _T("data/Sound/bgm/ClearBGM.mp3"));
		break;
	case 5:
		bgm.Open(hWnd, _T("data/Sound/bgm/Ending.mp3"));
		break;
	default:
		break;
	}
	bgm.Play(200, true);
}

void SoundPlayer::ItemPlay(int type)
{
	if (type == 0)
		OK.Play(200, false);
	else if (type == 1)
		H.Play(200, false);
	else if (type == 2)
		Thankyou.Play(500, false);
}

void SoundPlayer::EffectPlay(int type)
{
	if (type == 0)
		Shot.Play(300, false);
	else if (type == 1)
		Bomb.Play(220, false);
	else if (type == 2)
		H_Shot.Play(300, false, 0);
}

void SoundPlayer::DeathPlay(int type)
{
	if (type == 0)
		EriDeath.Play(200, false);
	if (type == 1)
		EnemyDeath.Play(300, false);
}

void SoundPlayer::ButtonPlay(int type)
{
	if (type == 0)
		Eri.Play(300, false);
}

void SoundPlayer::MissionPlay(int type)
{
	Once.Stop();
	switch (type)
	{
	case 0:
		Once.Open(hWnd, _T("data/Sound/Mission1.mp3"));
		break;
	case 1:
		Once.Open(hWnd, _T("data/Sound/Mission_Complete.mp3"));
		break;
	}
	Once.Play(300, false);
}


