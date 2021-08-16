#include "EnemyTrigger.h"
#include "ScenePlay.h"
#include "Helicopter.h"
#include "Berserker.h"
#include "Truck.h"
#include "Mosque.h"
#include "Keesi.h"
#include "Bradley.h"
#include <fstream>

EnemyTrigger::EnemyTrigger(ScenePlay *_scene, const char *path)
{
	scene = _scene;
	ifstream ifs;
	ifs.open(path);
	int n;
	ifs >> n;
	EnemyData temp;
	for (int i = 0; i < n; ++i)
	{
		ifs >> temp.phase >> temp.X >> temp.id >> temp.pos.X >> temp.pos.Y >> temp.item;

		temp.pos.X *= GameManager::instance()->zoom;
		temp.pos.Y *= GameManager::instance()->zoom;
		vData.push_back(temp);
	}
	ifs.close();
	curPhase = 0;
}

void EnemyTrigger::onTrigger(int X)
{
	static int curIdx = 0;
	static size_t i = 0;
	for (; i < vData.size(); ++i)
	{

		if (GameManager::instance()->phase < vData[i].phase)
			break;
		if (X < vData[i].X * GameManager::instance()->zoom)
			break;

		switch (vData[i].id)
		{
		case 0:
			scene->enemies.push_back(new Arabian(scene, vData[i].pos, vData[i].item));
			break;
		case 1:
			scene->enemies.push_back(new Berserker(scene, vData[i].pos, vData[i].item));
			break;
		case 2:
			scene->enemies.push_back(new Truck(scene, vData[i].pos, vData[i].item));
			break;
		case 4:
			scene->enemies.push_back(new Bradley(scene, vData[i].pos, vData[i].item));
			break;
		case 5:
			scene->enemies.push_back(new Mosque(scene, vData[i].pos, vData[i].item));
			break;
		case 6:
			scene->enemies.push_back(new Helicopter(scene, vData[i].pos, vData[i].item));
			break;
		case 7:
			scene->enemies.push_back(new Keesi(scene, vData[i].pos, vData[i].item));
			break;
		}
		scene->enemies.back()->Init();
	}
}


