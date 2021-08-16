#pragma once
#include "GameObjects.h"
#include "Stage.h"
#include "Animation.h"
#include "Projectile.h"
#include "Item.h"

class ScenePlay;

class Enemy : public GameObjects
{
protected:
	//Color color;
	ScenePlay* parent;
	int dist_ranged;
	int hp;
	int dropItemType;
	Animation* ani;
	bool human;
	bool hit;
	

public:
	Enemy(Scene* _scene, Point pos, int type);
	virtual void Init() {};
	virtual void Update();
	virtual void Draw(Graphics &);
	virtual void Hit(int);
	virtual void Attack();
	virtual void Transition(int _state) { state = _state; }
	int isInrange(Point pos);
	virtual int isDestroyed() { return state == -1; };
	virtual void isHit() {};
	bool isHuman() { return human; }
	void DropItem();
};

class Arabian : public Enemy
{

public:
	enum eState { DESTROY = -1, IDLE, RUNNING, DEATH, ATTACK, PRETHROW, THROW, JUMPBACK };

	Arabian(Scene *_scene, Point pos, int type);
	virtual void Init();
	virtual void Update();
	virtual void Draw(Graphics &);
	virtual void Hit(int);
	virtual void Attack();
	virtual void Transition(int _state);
	void isHit();
};


