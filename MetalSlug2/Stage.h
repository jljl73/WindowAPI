#pragma once
#include "GameObjects.h"
#include <vector>
#include "Camera.h"

using namespace std;

class ScenePlay;

class Stage : public GameObjects
{
	Image *sImg;
	Camera *camera;
	ScenePlay *parent;
public:
	
	vector<Line> ground;
	vector<Line> upperGround;
	vector<Line> wall;
	Stage();
	~Stage();
	void Init(Scene* scene);
	void Update();
	virtual void Draw(Graphics&);
};

