#include "Berserker.h"
#include "ScenePlay.h"
#include "Player.h"

Berserker::Berserker(Scene * _scene, Point pos, int type) : Enemy(_scene, pos, type)
{
	human = true;
	ani = new Animation[5];
	state = 0;
	//bLeft = true;
	dir = -1;
	hp = 5;
	dist_ranged = (w+h) * GameManager::instance()->zoom;
	w += 10 * GameManager::instance()->zoom;
}

void Berserker::Init()
{
	if (parent->berserkerImg == NULL)
		parent->berserkerImg = Image::FromFile(_T("data/Sprite/Enemy/Berserker_Arabian.png"));

	ani[0].Init(parent->berserkerImg, Color(255, 0, 255), Color(255, 0, 255), 0, 11,
		2, "data/Animation/Enemy/berserker_idle.txt");
	ani[1].Init(parent->berserkerImg, Color(255, 0, 255), Color(255, 0, 255), 0, 6,
		1, "data/Animation/Enemy/Berserker_Attack_lower.txt");
	ani[2].Init(parent->berserkerImg, Color(255, 0, 255), Color(255, 0, 255), 0, 6,
		1, "data/Animation/Enemy/berserker_Attack_upper.txt");
	ani[3].Init(parent->berserkerImg, Color(255, 0, 255), Color(255, 0, 255), 0, 6,
		1, "data/Animation/Enemy/Berserker_Running.txt");
	ani[4].Init(parent->berserkerImg, Color(255, 0, 255), Color(255, 0, 255), 0, 9,
		2, "data/Animation/Enemy/Berserker_death.txt");
}

void Berserker::Update()
{
	velocity.Y += 2;
	bool onGround = OnGravity(transform, velocity, parent->stage1->ground, 20)
		|| OnGravity(transform, velocity, parent->stage1->upperGround, 20);

	switch (state)
	{
	case IDLE:
		velocity.X = 0;
		break;
	case RUNNING:
	case ATTACK:
		velocity.X = 2 * GameManager::instance()->zoom * dir;
		break;
	case DEATH:
		if (onGround)
			velocity.X = 0;
	default:
		break;
	}

	transform = transform + velocity;
}

void Berserker::Draw(Graphics &graphics)
{
	int zoom = GameManager::instance()->zoom;

	velocity.Y += 2;
	Point position(transform.X - parent->camera->position().X, transform.Y - parent->camera->position().Y);

	switch (state)
	{
	case IDLE:
		if (ani[0].Play(graphics, position, dir > 0))
			Transition(RUNNING);
		break;
	case RUNNING:
		ani[3].Play(graphics, position, dir > 0);
		break;
	case ATTACK:
		ani[1].Play(graphics, position, dir > 0);
		if (ani[2].Play(graphics, position, dir > 0))
			Transition(RUNNING);
		break;
	case DEATH:
		if (ani[4].Play(graphics, position, dir > 0))
			Transition(DESTORYED);
		break;
	default:
		break;
	}


	//Pen pen(color);
	//graphics.DrawEllipse(&pen, Rect(position.X - (w >> 1)*zoom, position.Y - (h)*zoom,
	//	w * zoom, h * zoom));
}

void Berserker::Hit(int damage)
{
	Enemy::Hit(damage);

	if (isActive() && hp <= 0)
		Transition(DEATH);
}

void Berserker::Attack()
{
	if(state == RUNNING)
		Transition(ATTACK);
}

void Berserker::Transition(int _state)
{
	Enemy::Transition(_state);

	if (state == DEATH)
	{
		active = false;
		velocity.X = 5 * dir;
		velocity.X *= GameManager::instance()->zoom;
		velocity.Y = -10 * GameManager::instance()->zoom;
		DropItem();
	}

}

void Berserker::isHit()
{
	if (state == ATTACK)
	{
		if (Collision((GameObjects*)parent->player, (GameObjects*)this))
		{
			parent->player->Hit();
		}
	}
}
