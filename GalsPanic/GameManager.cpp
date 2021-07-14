#include "GameManager.h"
GameManager::GameManager()
{
	path.push_back(L"img/map1.jpg");
	path.push_back(L"img/map2.jpg");
	path.push_back(L"img/map3.jpg");
	path.push_back(L"img/map4.jpg");
}

// 초기화
void GameManager::Init()
{
	// UI
	oldTime = GetTickCount();
	gtime = 100000;
	ui.SetScreenRect(screenRect);
	
	Point p = { 100, 100 };
	map.push_back(Point(100, 100));

	// Player
	// 위치, 화면 크기
	player = make_unique<Player>(p, screenRect);

	// Enemy
	// 위치 속도 반지름

	for (int i = 0; i < 4 + select; ++i)
	{
		Point e = { 200 + rand() % 400, 200 + rand() % 400 };
		enemies.push_back(make_unique<Enemy>(e, 10, 5));
	}

	// Map
	p.X = 200;
	map.push_back(p);
	p.Y = 200;
	map.push_back(p);
	p.X = 100;
	map.push_back(p);
}

void GameManager::GameReset()
{
	map.points.clear();
	player.reset();
	enemies.clear();
	Init();
}

// 주로 키입력 담당 
void GameManager::Update()
{
	if (state != INGAME)
		return;

	DWORD newTime = GetTickCount();
	

	if (newTime - oldTime < 34)
	{
		return;
	}
	gtime -= newTime - oldTime;
	oldTime = newTime;

	static int count = 0;
	++count;
	if (count > 3)
	{
		player->NextFrame();
		for (int i = 0; i < enemies.size(); ++i)
			enemies[i]->NextFrame();
		count = 0;
	}

	// 키 입력
	player->ChangeSpace(0);
	if (GetKeyState(VK_SPACE) & 0x8000)
		player->ChangeSpace(1);

	player->BackTrack();

	if (GetKeyState(VK_LEFT) & 0x8000)
		player->Move(map, VK_LEFT);
	if (GetKeyState(VK_RIGHT) & 0x8000)
		player->Move(map, VK_RIGHT);
	if (GetKeyState(VK_UP) & 0x8000)
		player->Move(map, VK_UP);
	if (GetKeyState(VK_DOWN) & 0x8000)
		player->Move(map, VK_DOWN);


	for(int i = 0; i < enemies.size(); ++i)
	{
		enemies[i]->Move(map, *player);
		enemies[i]->CheckCollisionWithScreen(screenRect);
		//enemies[i]->CheckCollisionWithMap(map.points);

		if (isInPolygon(enemies[i]->getPos(), map.points))
			enemies.erase(enemies.begin() + i--);
	}

	score = getArea(map.points) * 100 / (screenRect.bottom - 10) / (screenRect.right - 10);
	if (score > 80)
	{
		Transition(CLEAR);
		GameReset();
	}
	if (gtime < 0)
	{
		Transition(OVER);
		GameReset();
	}
}
// 더블버퍼링 
void GameManager::DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc)
{
	HDC hMemDC;
	HBITMAP hOldBitmap;
	//int bx, by;


	hMemDC = CreateCompatibleDC(hdc);
	if (!hMemDC)
	{
		MessageBox(hWnd, _T("CreateCompatibleDC faild"),
			_T("Error"), MB_OK);
		return;
	}

	if (hDoubleBufferImage == NULL)
	{
		hDoubleBufferImage = CreateCompatibleBitmap(hdc, screenRect.right, screenRect.bottom);
	}

	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hDoubleBufferImage);
	GDI_Draw(hMemDC);

	BitBlt(hdc, 0, 0, screenRect.right, screenRect.bottom, hMemDC, 0, 0, SRCCOPY);
	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hMemDC);

}

void GameManager::DestroyEnemy()
{
	//enemy.reset();
	//enemy = nullptr;
}

void GameManager::Transition(int _state)
{
	state = _state;
	GameReset();
}

void GameManager::Select(int key)
{
	switch (key)
	{
	case VK_UP:
		select -= 2;
		break;
	case VK_DOWN:
		select += 2;
		break;
	case VK_LEFT:
		--select;
		break;
	case VK_RIGHT:
		++select;
		break;
	}
	if (select < 0)
		select += 4;
	else if (select > 3)
		select -= 4;
}

//GDI+
void GameManager::GDI_Init()
{
	GdiplusStartupInput gpsi;
	GdiplusStartup(&g_GdiToken, &gpsi, NULL);
}

void GameManager::GDI_Draw(HDC hdc)
{
	Graphics graphics(hdc);

	switch (state)
	{
	case START:
		DrawStartScreen(graphics);
		break;
	case SELECT:
		DrawSelectScreen(graphics);
		break;
	case INGAME:
		DrawGameScreen(graphics);
		break;
	case END:
		DrawEndScreen(graphics);
		break;
	case CLEAR:
		DrawClearScreen(graphics);
		break;
	case OVER:
		DrawOverScreen(graphics);
		break;
	}

}

void GameManager::GDI_End()
{
	GdiplusShutdown(g_GdiToken);
}

void GameManager::DrawStartScreen(Graphics &graphics)
{
	static int count = 0;

	Image* pImg = nullptr;

	if (count < 15)
		pImg = Image::FromFile((WCHAR*)L"img/Start1.jpg");
	else if (count < 30)
		pImg = Image::FromFile((WCHAR*)L"img/Start2.jpg");
	else
		count = 0;

	if (!pImg)
		return;

	int w = pImg->GetWidth();
	int h = pImg->GetHeight();
	graphics.DrawImage(pImg, 0, 0, w, h);

	++count;
	if (pImg)
		delete pImg;
}

void GameManager::DrawSelectScreen(Graphics & graphics)
{
	Image *pImg = Image::FromFile((WCHAR*)L"img/Empty.jpg");
	
	graphics.DrawImage(pImg, 0, 0, screenRect.right, screenRect.bottom);
	delete[] pImg;
	
	int w = 144;
	int h = 144;

	Pen pen(Color(255, 0, 255, 255));

	for (int i = 0; i < 4; ++i)
	{
		pImg = Image::FromFile(path[i]);

		graphics.DrawImage(pImg, (2 * (i % 2) + 1) * w, (2 * (i / 2) + 1) * h, w, h);
		if (i == select)
			graphics.DrawRectangle(&pen, (2 * (i % 2) + 1) * w, (2 * (i / 2) + 1) * h, w, h);
		delete[] pImg;
	}
}

void GameManager::DrawGameScreen(Graphics& graphics)
{
	map.DrawAll(graphics, path[select]);
	map.DrawOpen(graphics, screenRect, path[select]);
	player->Draw(graphics);

	for (auto enemy = enemies.begin(); enemy != enemies.end(); enemy++)
	{
		enemy->get()->Draw(graphics);
	}

	ui.DrawScore(graphics, score);
	ui.DrawHP(graphics, player->getHP());
	ui.DrawTime(graphics, gtime);
	if (player->getHP() < 1)
	{
		Transition(OVER);
	}
}

void GameManager::DrawClearScreen(Graphics& graphics)
{

	Image* pImg = nullptr;
	ImageAttributes imgAttr;
	imgAttr.SetColorKey(Color(255, 116, 110), Color(255, 116, 111));
	pImg = Image::FromFile((WCHAR*)L"img/goodjob.png");

	if (!pImg)
		return;

	int w = pImg->GetWidth();
	int h = pImg->GetHeight();

	graphics.DrawImage(pImg, Rect((screenRect.right / 2) - w, (screenRect.bottom / 2) - h, w * 2, h * 2),
		0, 0,
		w, h,
		UnitPixel, &imgAttr);

	if (pImg)
		delete pImg;
	static int ct = 0;
	if (ct++ == 30)
	{
		Transition(END);
		ct = 0;
	}
}

void GameManager::DrawOverScreen(Graphics& graphics)
{
	Image* pImg = nullptr;
	ImageAttributes imgAttr;
	imgAttr.SetColorKey(Color(255, 116, 110), Color(255, 116, 111));
	pImg = Image::FromFile((WCHAR*)L"img/gameover.png");

	if (!pImg)
		return;

	int w = pImg->GetWidth();
	int h = pImg->GetHeight();

	graphics.DrawImage(pImg, Rect((screenRect.right / 2)-w, (screenRect.bottom / 2) - h, w*2, h*2),
		0, 0,
		w, h,
		UnitPixel, &imgAttr);

	if (pImg)
		delete pImg;

	static int ct = 0;
	if (ct++ == 30)
	{
		Transition(START);
		ct = 0;
	}
}

void GameManager::DrawEndScreen(Graphics& graphics)
{
	map.DrawAll(graphics, path[select]);
	/*Image* pImg = nullptr;

	pImg = Image::FromFile((WCHAR*)L"img/End.jpg");

	if (!pImg)
		return;

	int w = pImg->GetWidth();
	int h = pImg->GetHeight();
	graphics.DrawImage(pImg, 0, 0, screenRect.right, screenRect.bottom);

	if (pImg)
		delete pImg;*/
}
