#pragma once
#include "Enemy.h"

class MosqueHead;
class Mosque : public Enemy
{
	MosqueHead *heads[3];
	enum MosqueSTATE { IDLE, PREPARE, ATTACK, EXPLOSION, DESTROY};
	vector<Point> vExplosion;

public:
	Mosque(Scene *_scene, Point pos, int);
	virtual void Init();
	virtual void Update();
	virtual void Draw(Graphics &);
	virtual void Transition(int _state);
	void Explosion(int n);
};


class MosqueHead : public Enemy
{
	DWORD oldTime;
public:
	MosqueHead(Scene *_scene, Point pos, int type);
	virtual void Hit(int);
	virtual void Update();
	virtual void Draw(Graphics &);
	virtual void Transition(int _state);
	virtual bool wrecked() {
		return state == 1;
	}
};

