#pragma once
#include "GameObjects.h"
#include "Animation.h"

class ScenePlay;

class Bomb : public GameObjects
{
	ScenePlay *parent;
	bool bounce;
public:
	Bomb(Scene *_scene, Point p, Point v);
	void Update();
	void Draw(Graphics &graphics);
	virtual void Transition(int _state);
	Animation ani[2];

};