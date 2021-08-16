#include "Projectile.h"
#include "ScenePlay.h"

Projectile::Projectile(ScenePlay * scene, Point pos, Point v, int _w, int _h, int _type)
	: GameObjects(pos, v, w, h), parent(scene)
{
	type = _type;
	if (type == 0)
	{
		ani = new Animation;
		ani[0].Init(parent->arabianImg, Color(255, 255, 255), Color(255, 255, 255), 0, 18,
			1, "data/Animation/Enemy/Arabian_SFX.txt");
	}
	else if (type == 1)
	{
		w = 40;
		h = 20;
		ani = new Animation[2];
		if (v.Y)
			ani[0].Init(parent->bradleyImg, Color(0, 248, 0), Color(0, 248, 0), 0, 12,
				1, "data/Animation/Enemy/Bradley_Missile.txt");
		else
			ani[0].Init(parent->bradleyImg, Color(0, 248, 0), Color(0, 248, 0), 0, 12,
				1, "data/Animation/Enemy/Bradley_Missile_Down.txt");

		ani[1].Init(parent->exploisionImg, Color(0, 248, 0), Color(0, 248, 0), 0, 16,
			0, "data/Animation/etc/ExplosionMid.txt");
	}
	else if (type == 2)
	{
		w = 15;
		h = 15;
		ani = new Animation;
		ani[0].Init(parent->helicopterImg, Color(255, 255, 255), Color(255, 255, 255), 0, 2,
			2, "data/Animation/Enemy/Helicopter_bullet.txt");
	}
}

void Projectile::Update()
{
	if (!isActive())
		return;

	if (type != 2)
	{
		velocity.Y += GameManager::instance()->zoom;;
		bool onGround = OnGravity(transform, velocity, parent->stage1->ground, 20);
		if (onGround)
		{
			active = false;
			velocity.X = 0;
			Transition(1);
		}
	}
	transform = transform + velocity;
}

void Projectile::Draw(Graphics & graphics)
{
	Point position(transform.X - parent->camera->position().X, transform.Y - parent->camera->position().Y);
	
	if (type == 0)
	{
		if (ani[0].Play(graphics, position, 0))
			Transition(1);
		if (state == 1)
			Transition(-1);
	}
	else if(type == 1)
	{
		if (state == 0 && ani[0].Play(graphics, position, 0))
			Transition(1);
		else if (state == 1 && ani[1].Play(graphics, position, 0))
			Transition(-1);
	}
	else
	{
		ani[0].Play(graphics, position, 0);
		
		if (state == 1)
			Transition(-1);
	}

	/*Pen pen(Color(0, 0, 255));
	graphics.DrawRectangle(&pen, Rect(position.X - (w >> 1)*GameManager::instance()->zoom, position.Y - (h)*GameManager::instance()->zoom,
		w * GameManager::instance()->zoom, h * GameManager::instance()->zoom));*/
}
