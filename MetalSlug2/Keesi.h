#pragma once
#include "Enemy.h"


class Keesi : public Enemy
{
	int fire_x1;
	int fire_x2;

	vector<Point> vExplosion;

public:
	enum eState { DESTROY = -1, IDLE, ATTACK, ATTACKEND, EXPLOSION};

	Keesi(Scene *_scene, Point pos, int type);
	virtual void Init();
	virtual void Update();
	virtual void Draw(Graphics &);
	virtual void Hit(int);
	virtual void Transition(int _state);
	virtual int isDestroyed() { return state == -1; };

	void isHit();
	void Summon();
	void Explosion(int n);
};

