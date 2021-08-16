#include "GameManager.h"
#include "ScenePlay.h"

//½Ì±ÛÅæ
GameManager* GameManager::_instance = NULL;
bool GameManager::instanceFlag = false;

GameManager::GameManager()
{
	sState = PLAY;
	//zoom = 3;
	screenWidth = 304 * zoom;
	screenHeight = 224 * zoom;
}

GameManager* GameManager::instance()
{
	if (instanceFlag == false)
	{
		_instance = new GameManager();
		instanceFlag = true;
	}

	return _instance;
}

void GameManager::GDI_Init()
{
	GdiplusStartupInput gpsi;
	GdiplusStartup(&g_GdiToken, &gpsi, NULL);

	scenePlay = new SceneStart();
	scenePlay->Init();
	soundplayer.Init();
}

void GameManager::GDI_Draw(HDC hdc)
{
	Graphics graphics(hdc);

	if (scenePlay)
		scenePlay->Draw(graphics);
}

void GameManager::GDI_End()
{
	GdiplusShutdown(g_GdiToken);
}

void GameManager::DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc)
{
	HDC hMemDC;
	HBITMAP hOldBitmap;

	hMemDC = CreateCompatibleDC(hdc);
	if (!hMemDC)
	{
		MessageBox(hWnd, _T("hMemDC error"), _T("Error"), MB_OK);
		return;
	}
	
	if (hDoubleBufferImage == NULL)
		hDoubleBufferImage = CreateCompatibleBitmap(hdc, screenWidth, screenHeight);

	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hDoubleBufferImage);
	GDI_Draw(hMemDC);

	BitBlt(hdc, 0, 0, screenWidth, screenHeight, hMemDC, 0, 0, SRCCOPY);
	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hMemDC);
}

void GameManager::Transition(int dest)
{
	delete scenePlay;
	sState = dest;
	switch (sState)
	{
	case MAIN:
		scenePlay = new SceneStart();
		break;
	case SELECT:
		scenePlay = new SceneSelect();
		break;
	case PLAY:
		scenePlay = new ScenePlay();
		break;
	case END:
		scenePlay = new SceneEnd();
	}
	scenePlay->Init();
}

void GameManager::Update()
{
	if (scenePlay)
		scenePlay->Update();
	if (scenePlay)
		scenePlay->LateUpdate();

}
