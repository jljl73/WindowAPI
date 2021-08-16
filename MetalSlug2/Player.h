#pragma once
#include "GameObjects.h"
#include "Gutil.h"
#include "Stage.h"
#include "Animation.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Bomb.h"
#include "GameManager.h"

class ScenePlay;


class Player : public GameObjects
{
	bool onGround;
	Image *pImg[5];
	Camera *camera;
	//Animation *curAni;
	Animation *ani;
	ScenePlay* parent;

	bool bScatter;
	bool lookup;
	bool crawl;
	bool HeavyMachineGun;
	void Transition(int _state);
	DWORD invincible;

public:
	enum pState { IDLE, DOWN, JUMP, DEATH, SHOT, KNIFE, BOMB, RESURRECTION};
	Player();
	~Player();
	void Init(Scene*);
	virtual void Draw(Graphics&);
	virtual void Update();

	void Hit();
	void Shot(Point p, Point v);

	void KeyManager();
	void AcquireItem(int type);
	void Resurrection();
};