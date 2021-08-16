#include "Mosque.h"
#include "ScenePlay.h"
#include "Projectile.h"
Mosque::Mosque(Scene * _scene, Point pos, int type) : Enemy(_scene, pos, type)
{
	ani = new Animation[5];
	active = false;
	w = 370;
	h = 100;
}

void Mosque::Init()
{
	if(parent->mosqueImg == NULL)
		parent->mosqueImg = Image::FromFile(_T("data/Sprite/Enemy/Mosque_Artillery.png"));

	ani[0].Init(parent->mosqueImg, Color(184, 224, 232), Color(184, 224, 232), 0, 2,
		2, "data/Animation/Enemy/Mosque_Base.txt");
	ani[1].Init(parent->mosqueImg, Color(184, 224, 232), Color(184, 224, 232), 0, 1,
		2, "data/Animation/Enemy/Mosque_tower.txt");
	ani[2].Init(parent->mosqueImg, Color(184, 224, 232), Color(184, 224, 232), 0, 3,
		2, "data/Animation/Enemy/Mosque_Open.txt");
	ani[3].Init(parent->mosqueImg, Color(184, 224, 232), Color(184, 224, 232), 0, 3,
		2, "data/Animation/Enemy/Mosque_Destoryed_Tower.txt");
	ani[3].Init(parent->mosqueImg, Color(184, 224, 232), Color(184, 224, 232), 0, 3,
		2, "data/Animation/Enemy/Mosque_Destoryed_Tower.txt");
	ani[4].Init(parent->exploisionImg, Color(0, 248, 0), Color(0, 248, 0), 0, 28,
		0, "data/Animation/etc/ExplosionBig.txt");
}

void Mosque::Update()
{
	static int ct = 0;
	switch (state)
	{
	case IDLE:
		if(transform.X < parent->player->Position().X)
			Transition(PREPARE);
		break;
	case PREPARE:
	{
		static int ct = 0;
		if (ct < 18)
		{
			parent->camera->MoveY(-1);
			++ct;
		}
		else
			Transition(ATTACK);
	}
		break;
	case ATTACK:
		if (heads[0]->wrecked() && heads[1]->wrecked() && heads[2]->wrecked())
		{
			Transition(EXPLOSION);
		}
		break;
	case EXPLOSION:
	{
		static DWORD oldTime = GetTickCount();
		DWORD curTime = GetTickCount();
		Explosion(5);

		if (curTime - oldTime > 6000)
			Transition(DESTROY);
	}
		break;
	}
}

void Mosque::Draw(Graphics &graphics)
{
	Point position(transform.X - parent->camera->position().X, transform.Y - parent->camera->position().Y);

	// 모가지

	switch (state)
	{
	case IDLE:
	case PREPARE:
		ani[1].Play(graphics, position, 0);
		break;
	case ATTACK:
	case EXPLOSION:
	{
		if (!heads[0]->wrecked())
			ani[2].Print(graphics, position, 0, 0);
		else
			ani[3].Print(graphics, position, 0, 0);

		if (!heads[1]->wrecked())
			ani[2].Print(graphics, position, 0, 1);
		else
			ani[3].Print(graphics, position, 0, 1);

		if (!heads[2]->wrecked())
			ani[2].Print(graphics, position, 0, 2);
		else
			ani[3].Print(graphics, position, 0, 2);
	}
		break;
	}

	// 베이스
	ani[0].Print(graphics, position, 0, state == DESTROY);

	//폭발
	for (int i = 0; i < vExplosion.size(); ++i)
	{
		Point temp(position.X + vExplosion[i].X, position.Y + vExplosion[i].Y);
		if (ani[4].Play(graphics, temp, 0))
			vExplosion.erase(vExplosion.begin() + i--);
	}

	
	/*Pen pen(Color(255, 0, 0));
	graphics.DrawEllipse(&pen, Rect(position.X - (w >> 1)*GameManager::instance()->zoom, position.Y - (h)*GameManager::instance()->zoom,
		w * GameManager::instance()->zoom, h * GameManager::instance()->zoom));*/
}

void Mosque::Transition(int _state)
{
	state = _state;

	if (state == PREPARE)
	{
		int zoom = GameManager::instance()->zoom;
		heads[0] = new MosqueHead(parent, Point(transform.X - 50 * zoom, 50 * zoom), -1);
		heads[1] = new MosqueHead(parent, Point(transform.X + 110, 50 * zoom), -1);
		heads[2] = new MosqueHead(parent, Point(transform.X + 140 * zoom, 50 * zoom), -1);

		parent->enemies.push_back(heads[0]);
		parent->enemies.push_back(heads[1]);
		parent->enemies.push_back(heads[2]);
	}
	else if (state == DESTROY)
	{
		DropItem();
	}
}

void Mosque::Explosion(int n)
{
	int zoom = GameManager::instance()->zoom;
	for (int i = vExplosion.size(); i < n; ++i)
	{
		Point temp((rand() % 300 - 100)*zoom, rand() % 100 * -zoom);
		vExplosion.push_back(temp);
		break;
	}
}

///////////////
/////////////////
//////////////////


MosqueHead::MosqueHead(Scene * _scene, Point pos, int type) : Enemy(_scene, pos, type) 
{
	hp = 30;
	w = 50;
	h = 50;
	ani = new Animation;
	oldTime = GetTickCount();
}

void MosqueHead::Hit(int damage)
{
	Enemy::Hit(damage);

	if (hp <= 0)
	{
		Transition(1);
	}
}

void MosqueHead::Update()
{
	DWORD curTime = GetTickCount();

	if (state == 0 && curTime - oldTime > 3000)
	{
		float theta = atan2(
			parent->player->Position().Y - transform.Y,
			parent->player->Position().X - transform.X );

		int speed = 7 * GameManager::instance()->zoom;
		Point v(speed * cos(theta), speed*sin(theta));

		parent->projectiles.push_back(new Projectile(parent, transform, v, 10, 10, 2));
		oldTime = curTime;
	}
}

void MosqueHead::Draw(Graphics &graphics)
{
	/*Point position(transform.X - parent->camera->position().X, transform.Y - parent->camera->position().Y);
	Pen pen(Color(255, 0, 0));
	graphics.DrawEllipse(&pen, Rect(position.X - (w >> 1)*GameManager::instance()->zoom, position.Y - (h)*GameManager::instance()->zoom,
		w * GameManager::instance()->zoom, h * GameManager::instance()->zoom));*/
}

void MosqueHead::Transition(int _state)
{
	state = _state;
	if (state == 1)
		active = false;
}

