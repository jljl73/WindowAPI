#pragma once
#include "Gutil.h"
#include "Map.h"
#include "Player.h"
#include <memory>

class Player;

class Enemy
{
protected:
	Point pos;
	Image* pImg;
	LONG speed;
	int radius;
	double dir;
	int type;
	int state; // 0 : Alive 1: Death 2: Delete
	int frame, maxframe;

public:
	Enemy(Point _pos, LONG _speed, int _radius, int _type) : pos(_pos),
		speed(_speed), radius(_radius) , type(_type), frame(0)
	{
		pImg = 0;
		dir = rand();
	}

	Point getPos() { return pos; }
	int getType() { return type; }
	int getState() { return state; }

	virtual void Draw(Graphics& graphics);
	virtual void Move(Map &, Player&);
	void NextFrame();
	void RandomMove();
	void CheckCollisionWithScreen(RECT&);
	void CheckCollisionWithMap(vector<Point>&polygon); // 사용 안함
	void CheckCollisionWithNewMap(Player &player, Point&);

	void Transition(int _state);
};

class Enemy_Red : public Enemy
{
public:
	Enemy_Red(Point _pos, LONG _speed, int _radius) : Enemy(_pos, _speed, _radius, 0)
	{
		maxframe = 13;
	}

	virtual void Draw(Graphics& graphics);
	virtual void Move(Map & map, Player& player);
};

class Enemy_Dia : public Enemy
{
public:
	Enemy_Dia(Point _pos, LONG _speed, int _radius) : Enemy(_pos, _speed, _radius, 1)
	{
		maxframe = 5;
	}

	virtual void Draw(Graphics& graphics);
	//virtual void Move(Map & map, Player& player);
};