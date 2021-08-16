#pragma once
#include "Enemy.h"

class Bradley : public Enemy
{
	enum BradleyState{ IDLE, WALK, RAISEDOWN, SHOOT, WRECKED, EXPLOSION};
	bool down;
public:
	Bradley(Scene* _scene, Point pos, int type);
	virtual void Init();
	virtual void Update();
	virtual void Draw(Graphics &);
	virtual void Hit(int);
	virtual void Transition(int _state);
};

