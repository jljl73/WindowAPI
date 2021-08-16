#pragma once
#include "Enemy.h"

class Berserker : public Enemy
{
	enum eState{ DESTORYED = -1, IDLE, RUNNING, ATTACK, DEATH};

public:
	Berserker(Scene *_scene, Point pos, int type);
	virtual void Init();
	virtual void Update();
	virtual void Draw(Graphics &);
	virtual void Hit(int);
	virtual void Attack();
	virtual void Transition(int _state);
	void isHit();
};