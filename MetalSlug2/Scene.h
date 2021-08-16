#pragma once
#include "Gutil.h"
#include "GameObjects.h"
#include "SoundPlayer.h"

class Animation;

class Scene
{
public:
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void LateUpdate() {}
	virtual void Draw(Graphics& graphics) = 0;

};

class SceneStart : public Scene
{
public:
	virtual void Init();
	virtual void Update();
	virtual void Draw(Graphics& graphics);
};

class SceneSelect : public Scene
{
	Animation *ani;
	Image *pImg;

	int cursor;
	int state;
public:
	virtual void Init();
	virtual void Update();
	virtual void Draw(Graphics& graphics);
};

class SceneEnd : public Scene
{
	Image *pImg;
	Animation *ani;
public:
	virtual void Init();
	virtual void Update();
	virtual void Draw(Graphics& graphics);
};
