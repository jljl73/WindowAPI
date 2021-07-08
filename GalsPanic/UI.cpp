#include "UI.h"

void UI::DrawScore(Graphics & graphics, int score)
{
	WCHAR text[64];
	SolidBrush brush(Color(255, 255, 0, 0));
	FontFamily fontfamily(L"Times New Roman");
	Font font(&fontfamily, 24, FontStyleRegular, UnitPixel);
	PointF pointF(580.0f, 20.0f);

	swprintf_s(text, L"Score %d%%", score);
	graphics.DrawString(text, -1, &font, pointF, &brush);
}

void UI::DrawHP(Graphics & graphics, int HP)
{
	Image *pImg;
	pImg = Image::FromFile((WCHAR*)L"img/hp.png");
	if (!pImg)
		return;
	int w = 30;
	int h = 30;
	for(int i = 0; i < HP; ++i)
		graphics.DrawImage(pImg, w*i, 0, w, h);
	if (pImg)
		delete[] pImg;
}
