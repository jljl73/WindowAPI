#pragma once
#include "GameObjects.h"
#include "Animation.h"
#include "Camera.h"

class ScenePlay;
class Bullet : public GameObjects
{
	ScenePlay *parent;
	int type;
public:
	Bullet(Scene *, Point _p, Point _v, int type);
	void Update();
	void Draw(Graphics &graphics);
	virtual void Transition(int _state);
	Animation ani[2];

};