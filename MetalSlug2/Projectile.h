#pragma once
#include "GameObjects.h"
#include "Animation.h"

class ScenePlay;

class Projectile : public GameObjects
{
	int type;
	Animation *ani;
	ScenePlay *parent;
public:
	Projectile(ScenePlay *scene, Point pos, Point v, int _w, int _h, int _type);
	~Projectile()
	{
		delete[] ani;
	}
	void Update();
	void Draw(Graphics &graphics);
};

