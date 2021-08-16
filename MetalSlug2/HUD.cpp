#include "HUD.h"

HUD::HUD()
{
	transform = Point(GameManager::instance()->screenWidth / 2, 20*GameManager::instance()->zoom);
	HUDImg = Image::FromFile(_T("data/UI/HUD.png"));
	ani[0].Init(HUDImg, Color(222, 0, 255), Color(222, 0, 255), 0, 1,
		100, "data/UI/HP_bar.txt");
	ani[1].Init(HUDImg, Color(222, 0, 255), Color(222, 0, 255), 0, 2,
		100, "data/UI/status.txt");
	ani[2].Init(HUDImg, Color(222, 0, 255), Color(222, 0, 255), 0, 10,
		100, "data/UI/Time_Number.txt");
	ani[3].Init(HUDImg, Color(222, 0, 255), Color(222, 0, 255), 0, 1,
		100, "data/UI/HP.txt");
	ani[4].Init(HUDImg, Color(222, 0, 255), Color(222, 0, 255), 0, 10,
		100, "data/UI/number.txt");
}
void HUD::Draw(Graphics & graphics)
{
	int zoom = GameManager::instance()->zoom;
	ani[0].Print(graphics, Point(13 * zoom, 23 * zoom), 0, 0); // Ã¼·Â¹Ù
	if(GameManager::instance()->nBullet == 0)
		ani[1].Print(graphics, Point(72 * zoom, 25 * zoom), 0, 0); // status
	else
		ani[1].Print(graphics, Point(72 * zoom, 25 * zoom), 0, 1); // status

	ani[2].Print(graphics, Point(135 * zoom, 25 * zoom), 0, 9); // ½Ã°£
	ani[2].Print(graphics, Point(151 * zoom, 25 * zoom), 0, 9); // ½Ã°£

	ani[3].Print(graphics, Point(16 * zoom, 32 * zoom), 0, 0); // HP
	ani[4].Print(graphics, Point(50 * zoom, 32 * zoom), 0, GameManager::instance()->nHP); // HP

	int score = GameManager::instance()->score;
	if(score == 0)
		ani[4].Print(graphics, Point(63 * zoom, 16 * zoom), 0, 0); // Á¡¼ö

	int ct = 0;
	while (score)
	{
		ani[4].Print(graphics, Point((63 - ct*8) * zoom, 16 * zoom), 0, score%10); // Á¡¼ö
		score /= 10;
		++ct;
	}
	ct = 0;
	int nBomb = GameManager::instance()->nBomb;
	while (1)
	{
		ani[4].Print(graphics, Point((119 - ct * 9) * zoom, 24 * zoom), 0, nBomb % 10); // ÆøÅº
		nBomb /= 10;
		if (!nBomb)
			break;
		++ct;
	}

	int nBullet = GameManager::instance()->nBullet;
	ct = 0;
	while (nBullet)
	{
		ani[4].Print(graphics, Point((95 - ct * 9) * zoom, 24 * zoom), 0, nBullet % 10); // ÃÑ¾Ë
		nBullet /= 10;
		++ct;
	}

	if (GameManager::instance()->clear)
	{
		Image *pImg = Image::FromFile(_T("data/UI/MissionComplete.png"));

		static int a = 0;
		SolidBrush brush(Color(a < 255? a : 255, 0, 0, 0));;
		a += 4;
		graphics.FillRectangle(&brush, 0, 0, GameManager::instance()->screenWidth, GameManager::instance()->screenHeight);
		if (a >= 360)
			GameManager::instance()->Transition(3);
		ImageAttributes imgAttr;
		imgAttr.SetColorKey(Color(0, 0, 0), Color(0, 0, 0));
		graphics.DrawImage(pImg, Rect(GameManager::instance()->screenWidth/2 - 380, 
			GameManager::instance()->screenHeight / 2 - 170, 762, 344), 0, 0, 762, 344, UnitPixel, &imgAttr);

		if(pImg)
			delete pImg;
	}
}
