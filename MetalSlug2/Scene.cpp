#include "Scene.h"
#include "GameManager.h"
#include "SoundPlayer.h"
#include "Animation.h"

void SceneStart::Init()
{
	GameManager::instance()->soundplayer.BGMPlay(1);
}

void SceneStart::Update()
{
	if (GetKeyState('1') & 0x8000)
		GameManager::instance()->Transition(1);
}

void SceneStart::Draw(Graphics & graphics)
{
	ImageAttributes imgAttr;

	Image *pImg = Image::FromFile(_T("data/screen.png"));
	graphics.DrawImage(pImg,
		Rect(0, 0, GameManager::instance()->screenWidth, GameManager::instance()->screenHeight),
		633, 1090,
		304, 240,
		UnitPixel, &imgAttr);

	if (pImg)
		delete pImg;
}

///////////////////////////
////////////SELECT/////////
///////////////////////////

void SceneSelect::Init()
{
	ani = new Animation[6];
	pImg = Image::FromFile(_T("data/Select.png"));
	GameManager::instance()->soundplayer.BGMPlay(2);

	ani[0].Init(pImg, Color(255, 255, 255), Color(255, 255, 255), 0, 1, 1,
		"data/Animation/UI/BG.txt");
	ani[1].Init(pImg, Color(255, 255, 255), Color(255, 255, 255), 0, 12, 1,
		"data/Animation/UI/Character.txt");
	ani[2].Init(pImg, Color(255, 255, 255), Color(255, 255, 255), 0, 4, 1,
		"data/Animation/UI/1P.txt");
	ani[3].Init(pImg, Color(255, 255, 255), Color(255, 255, 255), 0, 1, 1,
		"data/Animation/UI/Door.txt");
	ani[4].Init(pImg, Color(255, 255, 255), Color(255, 255, 255), 0, 1, 1,
		"data/Animation/UI/Selected.txt");
}

void SceneSelect::Update()
{
	if (state)
		return;
	static DWORD oldTime = 0;
	DWORD curTime = GetTickCount();
	if (curTime - oldTime < 60)
		return;
	oldTime = curTime;

	if ((GetKeyState('A') & 0x8000) && cursor == 1)
	{
		state = 1;
		GameManager::instance()->soundplayer.ButtonPlay(0);
	}

	if (GetKeyState(VK_LEFT) & 0x8000)
		--cursor;
	if (GetKeyState(VK_RIGHT) & 0x8000)
		++cursor;

	if (cursor < 0)
		cursor += 4;
	if (cursor >= 4)
		cursor -= 4;
}

void SceneSelect::Draw(Graphics & graphics)
{
	SolidBrush brush(Color(0,0,0));
	graphics.FillRectangle(&brush, 0, 0, GameManager::instance()->screenWidth, GameManager::instance()->screenHeight);

	for (int i = 0; i < 4; ++i)
	{
		if (i == cursor)
		{
			if(state == 1)
				ani[1].Print(graphics, Point(0, 0), 0, i + 8);
			else
				ani[1].Print(graphics, Point(0, 0), 0, i + 4);
		}
		else
			ani[1].Print(graphics, Point(0, 0), 0, i);
	}

	
	if (state == 1)
	{
		static int x = -400;
		if (x < 0)
			x += 40;
		else
		{
			Sleep(1500);
			GameManager::instance()->Transition(2);
			return;
		}
		ani[3].Print(graphics, Point(0, x), 0, 0);
		ani[4].Print(graphics, Point(0, x), 0, 0);
	}

	ani[0].Print(graphics, Point(0, 0), 0, 0);
	ani[2].Print(graphics, Point(0, 0), 0, cursor);
}

void SceneEnd::Init()
{
	ani = new Animation;
	pImg = Image::FromFile(_T("data/UI/ScoreNumber.png"));
	ani->Init(pImg, Color(255, 255, 255), Color(255, 255, 255), 0, 10, 0,
		"data/UI/ScoreNumber.txt");
	GameManager::instance()->soundplayer.BGMPlay(5);
}

void SceneEnd::Update()
{
}

void SceneEnd::Draw(Graphics & graphics)
{
	ImageAttributes imgAttr;
	SolidBrush brush(Color(0, 0, 0));
	graphics.FillRectangle(&brush, 0, 0, GameManager::instance()->screenWidth, GameManager::instance()->screenHeight);

	Image *pImg = Image::FromFile(_T("data/Clear1.png"));
	graphics.DrawImage(pImg,
		Rect(0, 0, GameManager::instance()->screenWidth, GameManager::instance()->screenHeight),
		0, 0,
		224, 192,
		UnitPixel, &imgAttr);


	if (pImg)
		delete pImg;

	int score = GameManager::instance()->score;
	int ct = 0;
	int zoom = GameManager::instance()->zoom;
	while (score)
	{
		ani->Print(graphics, Point((250 - ct * 47) * zoom, 125 * zoom), 0, score % 10); // Á¡¼ö
		score /= 10;
		++ct;
	}
}
