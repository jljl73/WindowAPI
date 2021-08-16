#pragma once
#include "Enemy.h"

class Helicopter : public Enemy
{
	vector<Point> bullet;

public:
	enum eState { DESTROY = -1, IDLE, DEATH, ATTACK, AFTER_ATTACK, EXPLOSION };

	Helicopter(Scene *_scene, Point pos, int type);
	virtual void Init();
	virtual void Update();
	virtual void Draw(Graphics &);
	virtual void Hit(int);
	virtual void Transition(int _state);
	virtual int isDestroyed() { return state == -1; };
	void isHit();
};