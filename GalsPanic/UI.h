#pragma once
#include "Gutil.h"
class UI
{
	RECT screenRect;
public:
	
	void SetScreenRect(RECT _rect) { screenRect = _rect; }
	void DrawScore(Graphics& graphics, int score);
	void DrawHP(Graphics& graphics, int HP);
	void DrawTime(Graphics & graphics, int time);
};