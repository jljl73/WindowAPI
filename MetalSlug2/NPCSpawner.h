#pragma once
#include "NPC.h"

class NPCSpawner
{
	struct NPCData
	{
		Point pos;
		int item;
		int dir;
	};
	ScenePlay* scene;

public:
	NPCSpawner(ScenePlay*, const char*);

};

