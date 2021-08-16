#pragma once
#include "GameObjects.h"
#include "GameManager.h"

class GameObjects;

class Camera : public GameObjects
{
	int maxX;
	int maxY;

public:
	Camera();
	
	vector<int> phaseX;
	int zoom;

	void Update();
	Point position() { return transform; }
	int LeftX() { return transform.X; }
	int MidX() { return transform.X + GameManager::instance()->screenWidth / 2;}
	int RightX() { return transform.X + GameManager::instance()->screenWidth; }
	void MoveX(int x);
	void MoveY(int y);

	void PhaseClear() { ++GameManager::instance()->phase; }
	bool EndPhase();

	bool isOut(GameObjects *object);
	bool isLeftOut(GameObjects *object);
};