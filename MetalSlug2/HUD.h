#pragma once
#include "Player.h"
#include "GameManager.h"

class HUD
{
	Point transform;
	Animation ani[10];
	Image *HUDImg;
public:
	HUD();
	void Update() {};
	void Draw(Graphics &graphics);
};