#include "ScenePlay.h"

#include "Stage.h"
#include "Player.h"
#include "Camera.h"
//#include "Enemy.h"

ScenePlay::~ScenePlay()
{
	if (player)
		delete player;
	if(camera)
		delete camera;
	if(stage1)
		delete stage1;

	if (bImg)
		delete bImg;

	if (boImg)
		delete boImg;
	if (exploisionImg)
		delete exploisionImg;
	if (arabianImg)
		delete arabianImg;
	if (helicopterImg)
		delete helicopterImg;
	if (keesiImg)
		delete keesiImg;

	if (berserkerImg)
		delete berserkerImg;
	if (bradleyImg)
		delete bradleyImg;
	if (truckImg)
		delete truckImg;
	if (mosqueImg)
		delete mosqueImg;
	if (itemImg)
		delete itemImg;
	if (npcImg)
		delete npcImg;

	if (ET)
		delete ET;
	if (NS)
		delete NS;
}

void ScenePlay::Init()
{
	ET = new EnemyTrigger(this, "data/Spawn/stage1.txt");
	NS = new NPCSpawner(this, "data/Spawn/stage1_NPC.txt");
	player = new Player;
	camera = new Camera;
	stage1 = new Stage;

	player->Init(this);
	stage1->Init(this);
	
	GameManager::instance()->soundplayer.BGMPlay(0);
	GameManager::instance()->soundplayer.MissionPlay(0);

	bImg = Image::FromFile(_T("data/Sprite/SFX/Weapon_SFX.png"));
	boImg = Image::FromFile(_T("data/Sprite/SFX/Other_SFX.png"));
	exploisionImg = Image::FromFile(_T("data/Sprite/SFX/Explosion_SFX.png"));
	helicopterImg = Image::FromFile(_T("data/Sprite/Enemy/Helicopter.png"));
}

void ScenePlay::Update()
{

	player->Update();
	stage1->Update();
	camera->Update();
	for (auto i : items)
		i->Update();

	for (size_t i = 0; i < enemies.size(); ++i)
	{
		enemies[i]->Update();

		if (enemies[i]->isInrange(player->Position()))
			enemies[i]->Attack();
	}


	for (size_t i = 0; i < npcs.size(); ++i)
	{
		npcs[i]->Update();
		if (npcs[i]->isSaved() && Collision(player, npcs[i]))
		{
			npcs[i]->Transition(2);
		}
	}

	for (size_t i = 0; i < pBullets.size(); ++i)
	{
		pBullets[i]->Update();
		if (!pBullets[i]->isActive())
			continue;

		for (size_t j = 0; j < enemies.size(); ++j)
		{
			if (!enemies[j]->isActive())
				continue;
			if (enemies[j]->isCollision(pBullets[i]->Position()))
			{
				enemies[j]->Hit(1);
				pBullets[i]->Transition(1);
				GameManager::instance()->score += 100;
				break;
			}
		}

		for (size_t j = 0; j < npcs.size(); ++j)
		{
			if (npcs[j]->isSaved())
				continue;
			if (npcs[j]->isCollision(pBullets[i]->Position()))
			{
				npcs[j]->Saved();
				pBullets[i]->Transition(1);
				break;
			}
		}
	}

	for (size_t i = 0; i < pBombs.size(); ++i)
	{
		pBombs[i]->Update();
		for (size_t j = 0; j < enemies.size(); ++j)
		{
			if (!enemies[j]->isActive())
				continue;
			if (Collision(enemies[j], pBombs[i]))
			{
				enemies[j]->Hit(10);
				pBombs[i]->Transition(1);
				GameManager::instance()->score += 100;
			}
		}

		for (size_t j = 0; j < npcs.size(); ++j)
		{
			if (npcs[j]->isSaved())
				continue;
			if (Collision(npcs[j], pBombs[i]))
			{
				npcs[j]->Saved();
				pBombs[i]->Transition(1);
				break;
			}
		}
	}

	for (size_t i = 0; i < items.size(); ++i)
	{
		if (Collision(player, items[i]))
		{
			player->AcquireItem(items[i]->GetType());
			delete items[i];
			items.erase(items.begin() + i--);
		}
	}

	for (size_t i = 0; i < projectiles.size(); ++i)
	{
		projectiles[i]->Update();
		if (Collision(player, projectiles[i]))
		{
			player->Hit();
			projectiles[i]->Transition(1);
			break;
		}
	}

	ET->onTrigger(player->Position().X);
}

void ScenePlay::LateUpdate()
{
	for (size_t i = 0; i < enemies.size(); ++i)
	{
		if (camera->isLeftOut(enemies[i]) || enemies[i]->isDestroyed())
		{
			delete enemies[i];
			enemies[i] = NULL;
			enemies.erase(enemies.begin() + i--);
		}
		else
			enemies[i]->isHit();
	}
	for (size_t i = 0; i < pBullets.size(); ++i)
	{
		if (camera->isOut(pBullets[i]) || pBullets[i]->isDestroyed())
		{
			delete pBullets[i];
			pBullets.erase(pBullets.begin() + i--);
		}
	}
	for (size_t i = 0; i < pBombs.size(); ++i)
	{
		if (camera->isOut(pBombs[i]) || pBombs[i]->isDestroyed())
		{
			delete pBombs[i];
			pBombs.erase(pBombs.begin() + i--);
		}
	}

	for (size_t i = 0; i < projectiles.size(); ++i)
	{
		if (camera->isOut(projectiles[i]) || projectiles[i]->isDestroyed())
		{
			delete projectiles[i];
			projectiles.erase(projectiles.begin() + i--);
		}
	}

	hud.Update();
}

void ScenePlay::Draw(Graphics & graphics)
{
	stage1->Draw(graphics);
	for (int i = 0; i < enemies.size(); ++i)
		enemies[i]->Draw(graphics);

	for (auto n : npcs)
		n->Draw(graphics);
	for (auto b : pBullets)
		b->Draw(graphics);
	for (auto b : pBombs)
		b->Draw(graphics);
	for (auto i : items)
		i->Draw(graphics);
	for (auto p : projectiles)
		p->Draw(graphics);
	player->Draw(graphics);
	hud.Draw(graphics);
}
