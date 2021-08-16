#pragma once
#include "Gutil.h"

class Scene;

class GameObjects
{
protected:
	Point transform;
	Point velocity;

	//HITBOX
	int w, h;
	//bool bLeft;
	int dir;
	int state;
	int active;

public:
	GameObjects() { active = true; }
	GameObjects(Point pos, Point v, int _w, int _h) : transform(pos), velocity(v), w(_w), h(_h) { active = true; }
	Point Position() { return transform; }

	int getW() { return w; }
	virtual void Init() {}
	virtual void Update() {}
	virtual void Draw(Graphics &) {}
	virtual void Transition(int _state) { state = _state; }
	virtual int isDestroyed() { return state == -1; }
	bool isActive() { return active; }
	

	virtual bool isCollision(Point q);
	friend bool Collision(GameObjects* p, GameObjects* q);
};

