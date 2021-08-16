#include "Bradley.h"
#include "ScenePlay.h"
#include "Projectile.h"

Bradley::Bradley(Scene * _scene, Point pos, int type) : Enemy(_scene, pos, type)
{
	ani = new Animation[10];
	w = 81;
	h = 50;
	hp = 50;
	state = WALK;
}

void Bradley::Init()
{
	if (parent->bradleyImg == NULL)
		parent->bradleyImg = Image::FromFile(_T("data/Sprite/Enemy/Bradley.png"));

	ani[0].Init(parent->bradleyImg, Color(255, 255, 255), Color(255, 255, 255), 0, 2,
		1, "data/Animation/Enemy/Bradley_IDLE.txt");
	ani[1].Init(parent->bradleyImg, Color(255, 255, 255), Color(255, 255, 255), 0, 2,
		1, "data/Animation/Enemy/Bradley_IDLE_Down.txt");
	ani[2].Init(parent->bradleyImg, Color(255, 255, 255), Color(255, 255, 255), 0, 8,
		1, "data/Animation/Enemy/Bradley_Walk.txt");
	ani[3].Init(parent->bradleyImg, Color(255, 255, 255), Color(255, 255, 255), 0, 9,
		1, "data/Animation/Enemy/Bradley_Raise.txt");
	ani[4].Init(parent->bradleyImg, Color(255, 255, 255), Color(255, 255, 255), 0, 9,
		1, "data/Animation/Enemy/Bradley_Down.txt");
	ani[5].Init(parent->bradleyImg, Color(255, 255, 255), Color(255, 255, 255), 0, 7,
		0, "data/Animation/Enemy/Bradley_Shooting.txt");
	ani[6].Init(parent->bradleyImg, Color(255, 255, 255), Color(255, 255, 255), 0, 7,
		0, "data/Animation/Enemy/Bradley_Shooting_Down.txt");
	ani[7].Init(parent->bradleyImg, Color(255, 255, 255), Color(255, 255, 255), 0, 20,
		0, "data/Animation/Enemy/Bradley_Wrecked.txt");
	ani[8].Init(parent->bradleyImg, Color(255, 255, 255), Color(255, 255, 255), 0, 20,
		0, "data/Animation/Enemy/Bradley_Wrecked_Down.txt");
	ani[9].Init(parent->exploisionImg, Color(0, 248, 0), Color(0, 248, 0), 0, 16,
		0, "data/Animation/etc/ExplosionHuge.txt");
}

void Bradley::Update()
{
	velocity.X = 0;
	hit = false;

	switch (state)
	{
	case WALK:
		velocity.X = -4 * GameManager::instance()->zoom;
	case SHOOT:
	{
		if(ani[5].CurFrame() == 6)
			parent->projectiles.push_back(
				new Projectile(parent, 
					Point(transform.X - w , transform.Y - h * GameManager::instance()->zoom),
					Point(-8 * GameManager::instance()->zoom, -8 * GameManager::instance()->zoom),
					20 * GameManager::instance()->zoom,
					20 * GameManager::instance()->zoom, 1));
		else if (ani[6].CurFrame() == 6)
			parent->projectiles.push_back(
				new Projectile(parent,
					Point(transform.X - w , transform.Y - (h-10) * GameManager::instance()->zoom),
					Point(-8 * GameManager::instance()->zoom, 0),
					20 * GameManager::instance()->zoom,
					20 * GameManager::instance()->zoom, 1));
	}
	break;
	default:
		break;
	}

	transform = transform + velocity;
}

void Bradley::Draw(Graphics &graphics)
{
	int zoom = GameManager::instance()->zoom;
	Point position(transform.X - parent->camera->position().X, transform.Y - parent->camera->position().Y);

	switch (state)
	{
	case IDLE:
		if (!down && ani[0].Play(graphics, position, dir < 0, hit))
			Transition(RAISEDOWN);
		else if (down && ani[1].Play(graphics, position, dir < 0, hit))
			Transition(RAISEDOWN);
		break;
	case WALK:
		if (ani[2].Play(graphics, position, dir < 0, hit))
			Transition(IDLE);
		break;
	case RAISEDOWN:
		if (down && ani[3].Play(graphics, position, dir < 0, hit))
		{
			Transition(SHOOT);
			down = false;
		}
		else if (!down && ani[4].Play(graphics, position, dir < 0, hit))
		{
			Transition(SHOOT);
			down = true;
		}
		break;
	case SHOOT:
		if (!down && ani[5].Play(graphics, position, dir < 0, hit, 6))
			Transition(IDLE);
		else if(down && ani[6].Play(graphics, position, dir < 0, hit, 6))
			Transition(IDLE);
		break;
	case WRECKED:
		if (!down && ani[7].Play(graphics, position, dir < 0, hit))
			Transition(EXPLOSION);
		else if (down && ani[8].Play(graphics, position, dir < 0, hit))
			Transition(EXPLOSION);
		break;
	case EXPLOSION:
		if (ani[9].Play(graphics, position, dir < 0, hit))
			Transition(-1);
	default:
		break;
	}


	//Pen pen(color);
	//graphics.DrawEllipse(&pen, Rect(position.X - (w >> 1)*zoom, position.Y - (h)*zoom,
	//	w * zoom, h * zoom));
}

void Bradley::Hit(int damage)
{
	Enemy::Hit(damage);
	if (hp <= 0)
	{
		active = false;
		Transition(WRECKED);
		DropItem();
	}
}

void Bradley::Transition(int _state)
{
	state = _state;
}
