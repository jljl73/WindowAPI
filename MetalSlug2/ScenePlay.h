#pragma once
#include "GameObjects.h"
#include "Stage.h"
#include "Scene.h"
#include "Enemy.h"
#include "Helicopter.h"
#include "NPCSpawner.h"
#include "EnemyTrigger.h"
#include "NPC.h"
#include <memory>
#include "HUD.h"
using namespace std;

class Player;

class ScenePlay : public Scene
{
public:

	HUD hud;

	Image *bImg;
	Image *boImg;
	Image *exploisionImg;

	Image *arabianImg;
	Image *helicopterImg;
	Image *keesiImg;
	Image *berserkerImg;
	Image *bradleyImg;
	Image *truckImg;
	Image *mosqueImg;

	Image *camelImg;
	Image *itemImg;
	Image *npcImg;

	Stage *stage1;
	Player *player;
	Camera *camera;
	vector<Enemy *> enemies;
	vector<GameObjects *> pBullets;
	vector<GameObjects *> pBombs;
	vector<GameObjects *> projectiles;
	vector<Item *> items;
	vector<NPC *> npcs;
	
	EnemyTrigger *ET;
	NPCSpawner *NS;


	ScenePlay() { helicopterImg = NULL; }
	~ScenePlay();

	void Init();
	void Update();
	void LateUpdate();
	void Draw(Graphics & graphics);

};