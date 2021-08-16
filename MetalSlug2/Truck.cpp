#include "Truck.h"
#include "Berserker.h"
#include "ScenePlay.h"
Truck::Truck(Scene * _scene, Point pos, int type) : Enemy(_scene, pos, type)
{
	ani = new Animation[4];
	w = 22 * GameManager::instance()->zoom;
	h *= GameManager::instance()->zoom;
	hp = 50;
}

void Truck::Init()
{
	if(parent->truckImg == NULL)
		parent->truckImg = Image::FromFile(_T("data/Sprite/Enemy/Rebel_Troop_Truck.png"));
	
	ani[0].Init(parent->truckImg, Color(255, 255, 255), Color(255, 255, 255), 0, 4,
		2, "data/Animation/Enemy/Truck_Driving.txt");
	ani[1].Init(parent->truckImg, Color(255, 255, 255), Color(255, 255, 255), 11, 12,
		1, "data/Animation/Enemy/Truck_Deploying.txt");
	ani[2].Init(parent->truckImg, Color(255, 255, 255), Color(255, 255, 255), 0, 6,
		2, "data/Animation/Enemy/Truck_Destroyed.txt");
	ani[3].Init(parent->exploisionImg, Color(0, 248, 0), Color(0, 248, 0), 0, 16,
		0, "data/Animation/etc/ExplosionHuge.txt");
	Transition(0);
}

void Truck::Update()
{
	hit = false;
	velocity.Y += 2;
	OnGravity(transform, velocity, parent->stage1->ground, 20);

	transform = transform + velocity;
}

void Truck::Draw(Graphics &graphics)
{
	if (parent->camera->isOut(this))
		return;

	int zoom = GameManager::instance()->zoom;
	Point position(transform.X - parent->camera->position().X, transform.Y - parent->camera->position().Y);

	switch (state)
	{
	case 0:
		if (ani[0].Play(graphics, position, dir < 0, hit))
		{
			Transition(1);
		}
		break;
	case 1:
		ani[1].Play(graphics, position, dir < 0, hit);
		break;
	case 2:
		ani[2].Play(graphics, position, dir < 0);
		break;
	case 3:
		ani[2].Play(graphics, position, dir < 0);
		if (ani[3].Play(graphics, position, dir < 0))
			Transition(2);
	}

	/*graphics.DrawRectangle(&pen, Rect(position.X - (w >> 1)*zoom, position.Y - (h)*zoom,
		w * zoom, h * zoom));*/
}

void Truck::Hit(int damage)
{
	Enemy::Hit(damage);
	if (hp <= 0)
	{
		Transition(3);
		active = false;
		w = 120 * 3;
	}
}


void Truck::Transition(int _state)
{
	Enemy::Transition(_state);
	if (state == 0)
		velocity.X = -5 * GameManager::instance()->zoom;
	else if (state == 1)
	{
		velocity.X = 0;
		AddGround();
		Spawn();
	}
	else if (state == 2)
	{
		DropItem();
		parent->stage1->wall.erase(parent->stage1->wall.begin());
	}
}

void Truck::Spawn()
{
	Point pos(transform.X, transform.Y - 24 * GameManager::instance()->zoom);
	Enemy* newEnemy = new Berserker(parent, pos, -1);
	newEnemy->Init();
	parent->enemies.insert(parent->enemies.begin(), newEnemy);
	//parent->enemies.push_back(newEnemy);
}

void Truck::AddGround()
{
	Line L1(Point(transform.X - (w >> 1) * GameManager::instance()->zoom,
		transform.Y - 20 * GameManager::instance()->zoom),
		Point(transform.X + (w >> 1) * GameManager::instance()->zoom,
			transform.Y - 20 * GameManager::instance()->zoom));

	Line L2(Point(transform.X + (w >> 1) * GameManager::instance()->zoom,
		transform.Y - 40 * GameManager::instance()->zoom),
		Point(transform.X + w * GameManager::instance()->zoom,
			transform.Y - 40 * GameManager::instance()->zoom)
	);

	parent->stage1->upperGround.push_back(L1);
	parent->stage1->upperGround.push_back(L2);

	L2.dest = L1.dest;
	L1.dest.X = L1.src.X;
	L1.dest.Y = transform.Y;

	parent->stage1->wall.push_back(L1);
	parent->stage1->wall.push_back(L2);
}
