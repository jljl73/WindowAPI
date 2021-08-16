#include "Keesi.h"
#include "ScenePlay.h"
#include "Player.h"
Keesi::Keesi(Scene * _scene, Point pos, int type) : Enemy(_scene, pos, type)
{
	ani = new Animation[7];
	fire_x1 = 80 * GameManager::instance()->zoom;
	fire_x2 = 100 * GameManager::instance()->zoom;
	hp = 100;
	w = 230;
	h = 80;
	hp = 299;

}

void Keesi::Init()
{
	if (parent->keesiImg == NULL)
		parent->keesiImg = Image::FromFile(_T("data/Sprite/Enemy/The_Keesi_2.png"));
	ani[0].Init(parent->keesiImg, Color(0, 248, 0), Color(0, 248, 0), 0, 6,
		3, "data/Animation/Enemy/Keesi_Idle.txt");
	ani[1].Init(parent->keesiImg, Color(0, 248, 0), Color(0, 248, 0), 0, 1,
		3, "data/Animation/Enemy/Keesi_Left.txt");
	ani[2].Init(parent->keesiImg, Color(0, 248, 0), Color(0, 248, 0), 0, 4,
		2, "data/Animation/Enemy/Keesi_left_fire.txt");
	ani[3].Init(parent->keesiImg, Color(0, 248, 0), Color(0, 248, 0), 0, 24,
		4, "data/Animation/Enemy/Keesi_Fire_attack_Left.txt");
	ani[4].Init(parent->keesiImg, Color(0, 248, 0), Color(0, 248, 0), 0, 15,
		2, "data/Animation/Enemy/Keesi_Fire_attack_Left2.txt");
	ani[5].Init(parent->exploisionImg, Color(0, 248, 0), Color(0, 248, 0), 0, 28,
		0, "data/Animation/etc/ExplosionBig.txt");
	ani[6].Init(parent->exploisionImg, Color(0, 248, 0), Color(0, 248, 0), 0, 16,
		0, "data/Animation/etc/ExplosionMid.txt");
	
	velocity.Y = 5;
	velocity.X = 5;
	GameManager::instance()->soundplayer.BGMPlay(3);
}

void Keesi::Update()
{
	hit = false;
	if (transform.Y > 100 * GameManager::instance()->zoom)
		velocity.Y = -5;
	else if(transform.Y < 90 * GameManager::instance()->zoom)
		velocity.Y = 5;
	if (transform.X < 3620 * GameManager::instance()->zoom)
		velocity.X = 4 * GameManager::instance()->zoom;
	else if (transform.X > 3744 * GameManager::instance()->zoom)
		velocity.X = -4 * GameManager::instance()->zoom;

	static DWORD oldTime = 0;
	DWORD curTime = GetTickCount();
	if (state != EXPLOSION && curTime - oldTime > 3000 && hp / 50)
	{
		Summon();
		oldTime = curTime;
	}

	Explosion(5 - hp / 50);

	if (state == EXPLOSION)
		transform.Y = transform.Y + 3 * GameManager::instance()->zoom;
	if (state != ATTACK)
		transform.Y = transform.Y + velocity.Y;
	else
		transform = transform + velocity;
}

void Keesi::Draw(Graphics &graphics)
{
	Point position(transform.X - parent->camera->position().X, transform.Y - parent->camera->position().Y);

	if (state != EXPLOSION)
	{
		ani[0].Print(graphics, position, dir < 0, hp / 50, hit);

		ani[1].Play(graphics, position, dir < 0, hit); //구녕
		ani[1].Play(graphics, position, 1, hit);
	}
	switch (state)
	{
	case IDLE:
	{
		ani[2].Play(graphics, position, dir < 0); // 불
		if(ani[2].Play(graphics, position, 1))
			Transition(ATTACK);
		break;
	}
	case ATTACK:
		ani[3].Play(graphics, position, 0); //공격
		if (ani[3].Play(graphics, position, 1))
			Transition(ATTACKEND);
		break;
	case ATTACKEND:
		ani[4].Play(graphics, position, 0); //공격종료
		if (ani[4].Play(graphics, position, 1))
			Transition(IDLE);
		break;
	case EXPLOSION:
		ani[5].Play(graphics, Point(position.X - 50 * GameManager::instance()->zoom, position.Y), 0);
		ani[5].Play(graphics, Point(position.X + 50 * GameManager::instance()->zoom, position.Y), 0);
		if (ani[5].Play(graphics, position, 0))
			Transition(-1);
		break;
	default:
		break;
	}

	for (int i = 0; i < vExplosion.size(); ++i)
	{
		Point temp(position.X + vExplosion[i].X, position.Y + vExplosion[i].Y);
		if (ani[6].Play(graphics, temp, 0))
			vExplosion.erase(vExplosion.begin() + i--);
	}

	/*Pen pen(Color(255, 0, 0));
	graphics.DrawEllipse(&pen, Rect(position.X - (w >> 1)*GameManager::instance()->zoom, position.Y - (h)*GameManager::instance()->zoom,
		w * GameManager::instance()->zoom, h * GameManager::instance()->zoom));*/
}

void Keesi::Hit(int damage)
{
	Enemy::Hit(damage);
	if (hp <= 0)
	{
		active = false;
		Transition(EXPLOSION);
	}
}

void Keesi::Transition(int _state)
{
	Enemy::Transition(_state);

	if (state == -1)
	{
		GameManager::instance()->clear = true;
		GameManager::instance()->soundplayer.BGMPlay(4);
		GameManager::instance()->soundplayer.MissionPlay(1);
	}
}

void Keesi::isHit()
{
	if (state == ATTACK)
	{
		int x = parent->player->Position().X;
		if (transform.X - fire_x2 <= x && transform.X - fire_x1 >= x)
			parent->player->Hit();
		else if (transform.X + fire_x1 <= x && transform.X + fire_x2 >= x)
			parent->player->Hit();
	}
}

void Keesi::Summon()
{
	if (parent->enemies.size() < 4)
	{
		Point pos(transform.X + rand()%400-200, 0);
		parent->enemies.push_back(new Arabian(parent, pos, -1));
		parent->enemies.back()->Init();
	}
}

void Keesi::Explosion(int n)
{
	int zoom = GameManager::instance()->zoom;
	for (int i = vExplosion.size(); i < n; ++i)
	{
		Point temp((rand() % 200 - 100)*zoom, rand() % 100 * -zoom);
		vExplosion.push_back(temp);
		break;
	}
}
