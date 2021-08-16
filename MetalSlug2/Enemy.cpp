#include "Player.h"
#include "Enemy.h"
#include "ScenePlay.h"

Enemy::Enemy(Scene * _scene, Point pos, int type)
{
	dropItemType = type;
	parent = (ScenePlay*)_scene;
	w = 32;
	h = 50;
	hp = 100;
	transform = pos;
	velocity = Point(0, 0);

}


void Enemy::Update()
{
	velocity.Y += GameManager::instance()->zoom;
	OnGravity(transform, velocity, parent->stage1->ground, 20);
	OnGravity(transform, velocity, parent->stage1->upperGround, 20);
}

void Enemy::Draw(Graphics &graphics)
{
}

void Enemy::Hit(int damage)
{
	hp -= damage;
	hit = true;
}

void Enemy::Attack()
{}

int Enemy::isInrange(Point pos)
{
	if (distance(transform, pos) < dist_ranged)
	{
		return 1;
	}
	return 0;
}

void Enemy::DropItem()
{
	if (dropItemType > -1)
		parent->items.push_back(new Item(parent, transform, dropItemType));
}

// 아라비안
Arabian::Arabian(Scene * _scene, Point pos, int type) : Enemy(_scene, pos, type)
{
	ani = new Animation[8];
	dist_ranged = GameManager::instance()->zoom * w;
	human = true;
}

void Arabian::Init()
{
	if(parent->arabianImg == NULL)
		parent->arabianImg = Image::FromFile(_T("data/Sprite/Enemy/Arabian.png"));

	ani[0].Init(parent->arabianImg, Color(255, 255, 255), Color(255, 255, 255), 0, 4,
		1, "data/Animation/Enemy/Arabian_Idle.txt");
	ani[1].Init(parent->arabianImg, Color(255, 255, 255), Color(255, 255, 255), 0, 12,
		1, "data/Animation/Enemy/Arabian_Running.txt");
	ani[2].Init(parent->arabianImg, Color(255, 255, 255), Color(255, 255, 255), 0, 13,
		0, "data/Animation/Enemy/Arabian_Death.txt");
	ani[3].Init(parent->arabianImg, Color(255, 255, 255), Color(255, 255, 255), 0, 8,
		1, "data/Animation/Enemy/Arabian_Attack.txt");

	ani[4].Init(parent->arabianImg, Color(255, 255, 255), Color(255, 255, 255), 0, 13,
		0, "data/Animation/Enemy/Arabian_Pre_Throw.txt");
	ani[5].Init(parent->arabianImg, Color(255, 255, 255), Color(255, 255, 255), 0, 13,
		0, "data/Animation/Enemy/Arabian_Throw.txt");
	ani[6].Init(parent->arabianImg, Color(255, 255, 255), Color(255, 255, 255), 0, 3,
		1, "data/Animation/Enemy/Arabian_Jump_Backwards.txt");

	state = 1;
	dir = -1;
}

void Arabian::Update()
{
	velocity.Y += 2;
	bool onGround = OnGravity(transform, velocity, parent->stage1->ground, 20)
	|| OnGravity(transform, velocity, parent->stage1->upperGround, 20);

	switch (state)
	{
	case IDLE://IDLE
	{
		velocity.X = 0;
	}
		break;
	case RUNNING://Running
	{
		int speed = 4 * GameManager::instance()->zoom * dir;
		velocity.X = speed;
		break;
	}
	case JUMPBACK:
	{
		if (onGround)
			Transition(0);
		break;
	}
	default:
		velocity.X = 0;
		break;
	}

	transform = transform + velocity;
}

void Arabian::Draw(Graphics &graphics)
{
	if (parent->camera->isOut(this))
		return;
	int zoom = GameManager::instance()->zoom;
	Point position(transform.X - parent->camera->position().X, transform.Y - parent->camera->position().Y);
	//Pen pen(color);
	/*graphics.DrawEllipse(&pen, Rect(position.X - (w >> 1)*zoom, position.Y - (h)*zoom,
		w * zoom, h * zoom));*/

	switch (state)
	{
	case IDLE:
		if (ani[0].Play(graphics, position, dir > 0)) // IDLE
		{
			if (rand() % 3)
				Transition(RUNNING);
			else
				Transition(THROW);
		}
		break;
	case RUNNING:
		if (ani[1].Play(graphics, position, dir > 0))
		{
			if ((parent->player->Position().X - transform.X) > 0)
				dir = 1;
			else
				dir = -1;
			Transition(IDLE);
		}
		break;
	case DEATH:
		if (ani[2].Play(graphics, position, dir > 0)) //Death
			Transition(-1);
		break;
	case ATTACK:
		if (ani[3].Play(graphics, position, dir > 0))
		{
			Transition(IDLE);
		}
		break;
	case PRETHROW:
		if (ani[4].Play(graphics, position, dir > 0)) // 원거리
			Transition(THROW);
	case THROW:
		if (ani[5].Play(graphics, position, dir > 0)) // 원거리
			Transition(IDLE);
		break;
	case JUMPBACK:
		ani[6].Play(graphics, position, dir > 0);
		break;
	default:
		break;
	}
}

void Arabian::Hit(int damage)
{
	Enemy::Hit(damage);
	Transition(2);
	GameManager::instance()->soundplayer.DeathPlay(1);
}

void Arabian::Attack()
{
	if (state == 2 || state < 0)
		return;

	if (state==0 || state ==1)
	{
		if (rand() % 4 == 0)
		{
			Transition(3);
		}
		else
			Transition(JUMPBACK);
	}
}

void Arabian::Transition(int _state)
{
	// Enter State

	if(state != _state)
		Enemy::Transition(_state);

	// Exit State
	if (state == DEATH)
	{
		active = false;
		DropItem();
	}

	if (state == JUMPBACK)
	{
		velocity.X = -5 * GameManager::instance()->zoom * dir;
		velocity.Y = -5 * GameManager::instance()->zoom;
	}
	if (state == ATTACK)
	{
		Point pos = transform;
		pos.X += (w)* dir;
		GameObjects* temp = new GameObjects(pos, Point(0, 0),
			(w << 1) * GameManager::instance()->zoom, h* GameManager::instance()->zoom);
		if (Collision((GameObjects*)parent->player, temp))
		{
			parent->player->Hit();
		}
		delete temp;
	}
	if (state == THROW)
	{
		Point temp(0, -12 * GameManager::instance()->zoom);
		temp.X = 3 * GameManager::instance()->zoom * dir;
		parent->projectiles.push_back(
			new Projectile(parent, Point(transform.X, transform.Y - 20),
				temp, 20 * GameManager::instance()->zoom, 20 * GameManager::instance()->zoom, 0));
	}
}

void Arabian::isHit()
{
	//if (state == ATTACK)
	//{
	//	Point pos = transform;
	//	pos.X += (w) * dir;
	//	GameObjects* temp = new GameObjects(pos, Point(0, 0),
	//		(w << 1) * GameManager::instance()->zoom, h* GameManager::instance()->zoom);
	//	if (Collision((GameObjects*)parent->player, temp))
	//	{
	//		parent->player->Hit();
	//	}
	//	delete temp;
	//}
}
