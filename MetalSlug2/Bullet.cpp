#include "Bullet.h"
#include "ScenePlay.h"

Bullet::Bullet(Scene*_scene, Point _p, Point _v, int _type)
{
	parent = (ScenePlay*)_scene;
	transform = _p;
	velocity = _v;
	type = _type;
	switch (type)
	{
	case 0: // ตüรั
		ani[0].Init(parent->bImg, Color(0, 248, 0), Color(0, 248, 0), 0, 1, 1, 
			"data/Animation/etc/bullet1.txt");
		break;
	case 1:
		ani[0].Init(parent->bImg, Color(0, 248, 0), Color(1, 248, 1), 0, 7, 1,
			"data/Animation/etc/H_bullet.txt");
		break;
	default:
		break;
	}

	ani[1].Init(parent->boImg, Color(254, 0, 254), Color(255, 1, 255), 0, 10, 0,
		"data/Animation/etc/Bullet_Destroy.txt");

}

void Bullet::Update()
{
	if (state == 1)
		return;

	transform = transform + velocity;
}

void Bullet::Draw(Graphics & graphics)
{
	int zoom = GameManager::instance()->zoom;
	Point position(transform.X - parent->camera->position().X, transform.Y - parent->camera->position().Y);

	if (state == 1)
	{
		if (ani[1].Play(graphics, position, 0))
			Transition(-1);
		return;
	}
	
	switch (type)
	{
	case 0:
		ani[0].Play(graphics, position, 0);
		break;
	case 1:
		if (velocity.Y == 0)
			ani[0].Print(graphics, position, velocity.X < 0, 0);
		else if (velocity.X == 0)
		{
			velocity.Y < 0 ?
				ani[0].Print(graphics, position, velocity.X < 0, 1) :
				ani[0].Print(graphics, position, velocity.X < 0, 2);
		}
		else if (abs(velocity.X) > abs(velocity.Y))
		{
			velocity.Y < 0 ?
				ani[0].Print(graphics, position, velocity.X < 0, 3) :
				ani[0].Print(graphics, position, velocity.X < 0, 5);
		}
		else
		{
			velocity.Y < 0 ?
				ani[0].Print(graphics, position, velocity.X < 0, 4) :
				ani[0].Print(graphics, position, velocity.X < 0, 6);
		}
		break;
	default:
		break;
	}

}

void Bullet::Transition(int _state)
{
	GameObjects::Transition(_state);
	if (state == 1)
		active = false;
}