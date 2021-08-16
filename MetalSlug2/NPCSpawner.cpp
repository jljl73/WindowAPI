#include "NPCSpawner.h"
#include "ScenePlay.h"

NPCSpawner::NPCSpawner(ScenePlay *_scene, const char *path)
{
	scene = _scene;
	ifstream ifs;
	ifs.open(path);
	int n;
	ifs >> n;
	NPCData temp;
	for (int i = 0; i < n; ++i)
	{
		ifs >> temp.pos.X >> temp.pos.Y >> temp.item >> temp.dir;
		temp.pos.X *= GameManager::instance()->zoom;
		temp.pos.Y *= GameManager::instance()->zoom;

		NPC* newNPC = new NPC(scene, temp.pos, temp.item, temp.dir);
		newNPC->Init();
		scene->npcs.push_back(newNPC);
	}
	ifs.close();
}
