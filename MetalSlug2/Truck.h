#pragma once
#include "Enemy.h"
class Truck : public Enemy
{
	enum TRUCKSTATE{ IDLE };

public:
	Truck(Scene *_scene, Point pos, int type);
	virtual void Init();
	virtual void Update();
	virtual void Draw(Graphics &);
	virtual void Hit(int);
	virtual void Transition(int _state);

	void Spawn();
	void AddGround();
};

