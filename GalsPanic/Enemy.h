#pragma once
#include "Gutil.h"
#include "Map.h"
#include "Player.h"
#include <memory>

class Player;

class Enemy
{
	Point pos;
	Image* pImg;
	LONG speed;
	int radius;
	double dir;
	int frame;

public:
	Enemy(Point _pos, LONG _speed, int _radius) : pos(_pos),
		speed(_speed), radius(_radius) , frame(0)
	{
		pImg = 0;
		dir = 1;
	}

	Point getPos() { return pos; }
	void Draw(Graphics& graphics);
	void NextFrame();
	void Move(Map &, Player&);
	void RandomMove();
	void CheckCollisionWithScreen(RECT&);
	void CheckCollisionWithMap(vector<Point>&polygon);
	void CheckCollisionWithNewMap(Player &player, Point&);
};