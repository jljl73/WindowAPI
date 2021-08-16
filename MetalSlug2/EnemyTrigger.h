#pragma once
#include "Enemy.h"

class ScenePlay;

class EnemyTrigger
{
	int idx = 0;
	int curPhase;
	ScenePlay* scene;
	struct EnemyData
	{
		int phase;
		int X;
		int id;
		Point pos;
		int item;
	};
	
	vector<EnemyData> vData;

public:

	EnemyTrigger(ScenePlay*, const char*);
	void onTrigger(int X);
};

