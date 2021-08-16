#include "Helicopter.h"
#include "ScenePlay.h"
#include "Player.h"

Helicopter::Helicopter(Scene * _scene, Point pos, int type) : Enemy(_scene, pos, type)
{
	ani = new Animation[5];
	w = 30 * GameManager::instance()->zoom;
	h = 20 * GameManager::instance()->zoom;
	hp = 50;
}

void Helicopter::Init()
{

	ani[0].Init(parent->helicopterImg, Color(255, 255, 255), Color(255, 255, 255), 0, 6,
		0, "data/Animation/Enemy/Helicopter_move.txt");;
	ani[1].Init(parent->helicopterImg, Color(255, 255, 255), Color(255, 255, 255), 0, 6,
		0, "data/Animation/Enemy/Helicopter_attack.txt");
	ani[2].Init(parent->helicopterImg, Color(255, 255, 255), Color(255, 255, 255), 0, 2,
		0, "data/Animation/Enemy/Helicopter_attack_SFX.txt");
	ani[3].Init(parent->helicopterImg, Color(255, 255, 255), Color(255, 255, 255), 0, 2,
		2, "data/Animation/Enemy/Helicopter_bullet.txt");
	ani[4].Init(parent->exploisionImg, Color(0, 248, 0), Color(0, 248, 0), 0, 16,
		0, "data/Animation/etc/ExplosionBig.txt");

	state = IDLE;
	velocity.X = 5 * GameManager::instance()->zoom;
}

void Helicopter::Update()
{
	for (size_t i = 0; i < bullet.size(); ++i)
	{
		bullet[i].Y += 5 * GameManager::instance()->zoom;
		if (bullet[i].Y > 600)
			bullet.erase(bullet.begin() + i--);
	}

	if (!isActive())
		return;

	hit = false;
	static Point bulletVelocity = Point(0, 5);

	if (transform.X < 3200 * GameManager::instance()->zoom)
	{
		velocity.X = 5 * GameManager::instance()->zoom;
		Transition(IDLE);
		dir = 1;
	}
	else if (transform.X > 3500 * GameManager::instance()->zoom)
	{
		velocity.X = -5 * GameManager::instance()->zoom;
		Transition(ATTACK);
		dir = -1;
	}

	switch (state)
	{
	case AFTER_ATTACK:
		break;
	default:
		break;
	}

	transform = transform + velocity;

}

void Helicopter::Draw(Graphics &graphics)
{

	Point position(transform.X - parent->camera->position().X, transform.Y - parent->camera->position().Y);
	switch (state)
	{
	case IDLE:
		ani[0].Play(graphics, position, dir > 0, hit);
		break;
	case ATTACK:
		ani[1].Play(graphics, position, dir < 0, hit);
		if (ani[2].Play(graphics, position, dir < 0, hit))
		{
			bullet.push_back(transform);
			Transition(AFTER_ATTACK);
		}
		break;
	case AFTER_ATTACK:
	{
		if(ani[1].Play(graphics, position, dir < 0, hit))
			Transition(ATTACK);
		break;
	}
	case EXPLOSION:
		if (ani[4].Play(graphics, position, dir < 0))
			Transition(-1);
		break;
	}
	/*Pen pen(Color(0, 0, 255));
	graphics.DrawRectangle(&pen, Rect(position.X - (w >> 1)*GameManager::instance()->zoom, position.Y - (h)*GameManager::instance()->zoom,
		w * GameManager::instance()->zoom, h * GameManager::instance()->zoom));*/

	for (auto b : bullet)
	{
		Point bullet_position(b.X - parent->camera->position().X, b.Y - parent->camera->position().Y);
		ani[3].Play(graphics, bullet_position, dir < 0);
	}

}

void Helicopter::Hit(int damage)
{
	Enemy::Hit(damage);
	if (hp <= 0)
	{
		active = false;
		Transition(EXPLOSION);
		DropItem();
	}
}

void Helicopter::Transition(int _state)
{
	Enemy::Transition(_state);
}

void Helicopter::isHit()
{
	GameObjects* player = (GameObjects*)parent->player;
	for(auto b : bullet)
	{
		if (player->isCollision(b))
		{
			parent->player->Hit();
		}
	}
}

