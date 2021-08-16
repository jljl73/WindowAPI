#pragma once
#include "GameObjects.h"
#include "Animation.h"

class ScenePlay;

class Item : public GameObjects
{
	enum ITEMIDX { BOMBBOX, H, SCORE };
	Animation *ani;
	int type;
	ScenePlay *parent;

public:
	Item(Scene* _scene, Point pos, int type);
	~Item() { if (ani) delete ani; }
	virtual void Draw(Graphics &);
	virtual void Update();
	int GetType() { return type; }
};

