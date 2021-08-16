#include "Player.h"
#include "ScenePlay.h"

Player::Player()
{
	state = IDLE;
	onGround = false;
	transform = Point(100, 100*GameManager::instance()->zoom);
	velocity = Point(0, 0);
	w = 32;
	h = 32;
	GameManager::instance()->nBomb = 10;
	GameManager::instance()->nHP = 2;
	HeavyMachineGun = false;
	dir = 1;
}

Player::~Player()
{
	if (pImg)
		delete pImg;
	if (ani)
		delete[] ani;
}

void Player::Init(Scene* _scene)
{
	ani = new Animation[25];
	//pImg = Image::FromFile(_T("data/Sprite/Player/Fio.jpg"));
	pImg[0] = Image::FromFile(_T("data/Sprite/Player/Eri_with_pistol.png"));
	pImg[1] = Image::FromFile(_T("data/Sprite/Player/Eri_death.png"));
	pImg[2] = Image::FromFile(_T("data/Sprite/Player/Eri_with_Heavy_Weapon.png"));
	parent = (ScenePlay*)_scene;
	camera = (Camera*)parent->camera;

	ani[0].Init(pImg[0], Color(255, 255, 255), Color(255, 255, 255), 0, 1, 2, // frame, ct
		"data/animation/Player/idle_lower.txt");
	ani[1].Init(pImg[0], Color(255, 255, 255), Color(255, 255, 255), 0, 4, 3,
		"data/animation/Player/idle_upper.txt");

	ani[2].Init(pImg[0], Color(255, 255, 255), Color(255, 255, 255), 0, 12, 0, // frame, ct
		"data/animation/Player/run_lower.txt");
	ani[3].Init(pImg[0], Color(255, 255, 255), Color(255, 255, 255), 0, 12, 0,
		"data/animation/Player/run_upper.txt");
	ani[4].Init(pImg[0], Color(255, 255, 255), Color(255, 255, 255), 0, 10, 0,
		"data/animation/Player/pistol_shot_upper.txt");
	ani[5].Init(pImg[0], Color(255, 255, 255), Color(255, 255, 255), 0, 6, 0,
		"data/animation/Player/knife_upper.txt");
	ani[6].Init(pImg[1], Color(255, 255, 255), Color(255, 255, 255), 0, 40, 0,
		"data/animation/Player/Death.txt");

	ani[7].Init(pImg[0], Color(255, 255, 255), Color(255, 255, 255), 0, 4, 3,
		"data/animation/Player/lookup_upper.txt");
	ani[8].Init(pImg[0], Color(255, 255, 255), Color(255, 255, 255), 0, 8, 0,
		"data/animation/Player/pistol_shot_lookup_upper.txt");

	ani[9].Init(pImg[0], Color(255, 255, 255), Color(255, 255, 255), 7, 11, 2,
		"data/animation/Player/Crawl.txt");
	ani[10].Init(pImg[0], Color(255, 255, 255), Color(255, 255, 255), 0, 7, 2,
		"data/animation/Player/Crawl_walk.txt");
	ani[11].Init(pImg[0], Color(255, 255, 255), Color(255, 255, 255), 0, 9, 0,
		"data/animation/Player/Crawl_Shot.txt");

	ani[12].Init(pImg[0], Color(255, 255, 255), Color(255, 255, 255), 0, 6, 0,
		"data/animation/Player/Throw_Bomb_upper.txt");
	
	// 헤비머신건
	ani[13].Init(pImg[2], Color(255, 255, 255), Color(255, 255, 255), 0, 4, 3,
		"data/animation/Player/H_Idle_upper.txt");
	ani[14].Init(pImg[2], Color(255, 255, 255), Color(255, 255, 255), 0, 12, 1,
		"data/animation/Player/H_Run_upper.txt");
	ani[15].Init(pImg[2], Color(255, 255, 255), Color(255, 255, 255), 0, 4, 1,
		"data/animation/Player/H_Crawl.txt");
	ani[16].Init(pImg[2], Color(255, 255, 255), Color(255, 255, 255), 0, 7, 1,
		"data/animation/Player/H_Crawl_Walk.txt");
	ani[17].Init(pImg[2], Color(255, 255, 255), Color(255, 255, 255), 0, 4, 1,
		"data/animation/Player/H_Crawl_Shot.txt");
	ani[18].Init(pImg[2], Color(255, 255, 255), Color(255, 255, 255), 0, 6, 0,
		"data/animation/Player/H_Shot_upper.txt");
	
	ani[19].Init(pImg[0], Color(255, 255, 255), Color(255, 255, 255), 0, 8, 0,
		"data/animation/Player/Crawl_Bomb.txt");
	ani[20].Init(pImg[0], Color(255, 255, 255), Color(255, 255, 255), 0, 6, 0,
		"data/animation/Player/Jump_Down.txt");
	ani[21].Init(pImg[2], Color(255, 255, 255), Color(255, 255, 255), 2, 6, 0,
		"data/animation/Player/H_Scatter_UP.txt");
	ani[22].Init(pImg[2], Color(255, 255, 255), Color(255, 255, 255), 2, 6, 0,
		"data/animation/Player/H_Scatter_Down.txt");
	ani[23].Init(pImg[0], Color(255, 255, 255), Color(255, 255, 255), 0, 6, 0,
		"data/animation/Player/Ressurection.txt");

	Resurrection();
}


void Player::Draw(Graphics &graphics)
{
	if (!pImg)
		return;

	int zoom = GameManager::instance()->zoom;
	Point position(
		transform.X - camera->position().X,		// X
		transform.Y - camera->position().Y		// Y
	);
	
	switch (state)
	{
	case -1:
		Resurrection();
		break;
	case IDLE:
		if (velocity.X == 0)
		{
			if(crawl)
				HeavyMachineGun ? ani[15].Play(graphics, position, dir < 0) : ani[9].Play(graphics, position, dir < 0);
			else
			{
				ani[0].Play(graphics, position, dir < 0);
				if (HeavyMachineGun)
				{
					ani[13].Play(graphics, position, dir < 0);
				}
				else
				{
					lookup ? ani[7].Play(graphics, position, dir < 0) : ani[1].Play(graphics, position, dir < 0);
				}
			}
		}
		else
		{
			if(crawl)
				HeavyMachineGun ? ani[16].Play(graphics, position, dir < 0) : ani[10].Play(graphics, position, dir < 0);
			else
			{
				ani[2].Play(graphics, position, dir < 0);

				if (HeavyMachineGun)
					ani[14].Play(graphics, position, dir < 0);
				else
					lookup ? ani[7].Play(graphics, position, dir < 0) : ani[3].Play(graphics, position, dir < 0);
			}
		}
		break;
	case KNIFE:
		if (velocity.X == 0)
			ani[0].Play(graphics, position, dir < 0);
		else
			ani[2].Play(graphics, position, dir < 0);
		if (ani[5].Play(graphics, position, dir < 0))
			Transition(IDLE);
		break;
	case SHOT:
	{
		if (velocity.X == 0)
			ani[0].Play(graphics, position, dir < 0);
		else
			ani[2].Play(graphics, position, dir < 0);

		if (lookup)
		{
			if (HeavyMachineGun)
			{
				if (ani[21].Play(graphics, position, dir < 0))
					Transition(IDLE);
			}
			else
			{
				if (ani[8].Play(graphics, position, dir < 0))
					Transition(IDLE);
			}
		}
		else if (crawl) // 아래버튼
		{
			if (onGround) //땅위에서
			{
				if (HeavyMachineGun)
				{
					if (ani[17].Play(graphics, position, dir < 0))
						Transition(IDLE);
				}
				else
				{
					if (ani[11].Play(graphics, position, dir < 0))
						Transition(IDLE);
				}
				ani[22].reset();
			}
			else // 점프
			{
				if (HeavyMachineGun)
				{
					if (ani[22].Play(graphics, position, dir < 0))
						Transition(IDLE);
				}
				else
				{
					if (ani[20].Play(graphics, position, dir < 0))
						Transition(IDLE);
				}

			}
		}
		else
		{
			if (HeavyMachineGun)
			{
				if (ani[18].Play(graphics, position, dir < 0))
					Transition(IDLE);
			}
			else
			{
				if (ani[4].Play(graphics, position, dir < 0))
					Transition(IDLE);
			}
			ani[21].reset();
		}
	}
	break;
	case BOMB:
		if (velocity.X == 0)
			ani[0].Play(graphics, position, dir < 0);
		else
			ani[2].Play(graphics, position, dir < 0);

		if (crawl)
		{
			if (ani[19].Play(graphics, position, dir < 0))
				Transition(IDLE);
		}
		else
		{
			if (ani[12].Play(graphics, position, dir < 0))
				Transition(IDLE);
		}
		break;
	case DEATH:
		if (ani[6].Play(graphics, position, dir < 0))
			Transition(-1);
		break;
	case RESURRECTION:
		if (ani[23].Play(graphics, position, dir < 0))
			Transition(IDLE);
		break;
	}

	/*Pen pen(Color(0, 0, 255));
	graphics.DrawEllipse(&pen, Rect(position.X - (w>>1)*zoom, position.Y - (h)*zoom, 
		w * zoom, h * zoom));*/
}

void Player::Update()
{

	int zoom = GameManager::instance()->zoom;

	KeyManager();

	if (crawl)
		h = 16;
	else
		h = 32;

	// 중력
	velocity.Y += zoom;
	if(velocity.Y > 0)
	onGround = OnGravity(transform, velocity, parent->stage1->ground, 20)
		|| OnGravity(transform, velocity, parent->stage1->upperGround, 20);


	OnWall(transform, velocity, parent->stage1->wall, (w>>1));


	// 위치 = 위치 + 속도*dt
	transform = transform + velocity;

	// 카메라 안 벗어나게
	if (transform.X < w + camera->LeftX())
		transform.X = w + camera->LeftX();
	if (transform.X > camera->RightX() - w)
		transform.X = camera->RightX() - w;


	// 카메라 이동
	if (transform.X > camera->MidX() - 100)
	{
		camera->MoveX(5*zoom);
	}
	else {
		camera->MoveX(0);
	}
}


void Player::Hit()
{
	int curTime = GetTickCount();
	if (isActive() && state != DEATH && curTime - invincible > 6000)
	{
		Transition(DEATH);
		active = false;
	}
}

void Player::Shot(Point p, Point v)
{
	Bullet * newBullet;
	newBullet = new Bullet(parent, p, v, HeavyMachineGun);
	parent->pBullets.push_back(newBullet);
	if (HeavyMachineGun)
	{
		if (--GameManager::instance()->nBullet <= 0)
			HeavyMachineGun = false;
	}
}

void Player::Transition(int _state)
{
	state = _state;
	if (state == SHOT)
	{
		ani[4].reset();
		ani[8].reset();
		ani[11].reset();
	}
	if (state == DEATH)
	{
		--GameManager::instance()->nHP;
		GameManager::instance()->soundplayer.DeathPlay(0);
	}
}

void Player::KeyManager()
{
	int zoom = GameManager::instance()->zoom;
	velocity.X = 0;
	if (!isActive() || state == RESURRECTION)
		return;

	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		velocity.X = -5 * zoom;
		dir = -1;
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		velocity.X = 5 * zoom;
		dir = 1;
	}
	lookup = false;
	if (GetKeyState(VK_UP) & 0x8000)
		lookup = true;

	crawl = false;
	if (GetKeyState(VK_DOWN) & 0x8000)
		crawl = true;

	if (onGround && crawl && (state == SHOT || state == BOMB))
		velocity.X = 0;

	// A 공격
	if (GetAsyncKeyState('A') & 0x8000)
	{

		bool inDistance = false;
		for (size_t i = 0; i < parent->enemies.size(); ++i)
		{
			if (parent->enemies[i]->isActive() && parent->enemies[i]->isHuman()
				&& Collision(this, parent->enemies[i]))
			{
				inDistance = true;
				Enemy* e = (Enemy*)parent->enemies[i];
				e->Hit(10);
			}
		}

		static int cooltime = 0;
		int curTime = GetTickCount();

		if (inDistance && curTime - cooltime > 90)
		{
			Transition(KNIFE);
		}
		else if (!HeavyMachineGun && curTime - cooltime > 90)
		{
			int speed = zoom * 25;
			if (lookup)
				Shot(Point(transform.X - 2 * zoom, transform.Y - (h + 10) * zoom), Point(0, -speed));
			else if (crawl)
			{
				onGround ?
					Shot(Point(transform.X, transform.Y - 16 * zoom), Point(speed * dir, 0)) :
					Shot(Point(transform.X, transform.Y), Point(0, speed));
			}
			else
			{
				speed *= dir;
				Shot(Point(transform.X, transform.Y - 24 * zoom), Point(speed, 0));
			}

			Transition(SHOT);
			GameManager::instance()->soundplayer.EffectPlay(0);
		}
		else if (HeavyMachineGun && curTime - cooltime > 30)
		{
			int speed = zoom * 25;
			if (lookup) // 위보고
			{
				if (ani[21].CurFrame() == 0)
					Shot(Point(transform.X - 6 * zoom + (curTime % 12), transform.Y), Point(speed*dir * 0.86, -speed * 0.5));
				else if (ani[21].CurFrame() == 1)
					Shot(Point(transform.X - 6 * zoom + (curTime % 12), transform.Y), Point(speed*dir * 0.5, -speed * 0.86));
				else
					Shot(Point(transform.X - 6 * zoom + (curTime % 12), transform.Y - (h + 10) * zoom), Point(0, -speed));
			}
			else if (crawl) // 앉아서
			{
				if (!onGround)
				{
					if (ani[22].CurFrame() == 0)
						Shot(Point(transform.X - 6 * zoom + (curTime % 12), transform.Y), Point(speed*dir * 0.86, speed * 0.5));
					else if (ani[22].CurFrame() == 1)
						Shot(Point(transform.X - 6 * zoom + (curTime % 12), transform.Y), Point(speed*dir * 0.5, speed * 0.86));
					else
						Shot(Point(transform.X + (curTime % 12), transform.Y), Point(0, speed));
				}
				else
				{
					speed *= dir;
					Shot(Point(transform.X + speed, transform.Y - 18 * zoom + (curTime % 12)), Point(speed, 0));
				}
			}
			else // IDLE
			{
				speed *= dir;
				Shot(Point(transform.X + speed, transform.Y - 16 * zoom + (curTime % 12)), Point(speed, 0));
			}


			Transition(SHOT);
			if(HeavyMachineGun)
				GameManager::instance()->soundplayer.EffectPlay(2);
			else
				GameManager::instance()->soundplayer.EffectPlay(0);
		}
		cooltime = curTime;
	}

	// B 점프
	if (onGround && (GetKeyState('S') & 0x8000))
	{
		onGround = false;
		velocity.Y = -12 * zoom;
	}

	// C 폭탄
	if (GetKeyState('D') & 0x8000)
	{
		static int cooltime = 0;
		int curTime = GetTickCount();
		if (GameManager::instance()->nBomb && curTime - cooltime > 500)
		{
			Bomb *newBomb = new Bomb(parent, transform, Point(5 * zoom * dir, -10 * zoom));
			parent->pBombs.push_back(newBomb);
			cooltime = curTime;
			--GameManager::instance()->nBomb;
		}
		Transition(BOMB);
	}
}

void Player::AcquireItem(int type)
{
	if (type == 0)
	{
		GameManager::instance()->nBomb += 10;
		GameManager::instance()->soundplayer.ItemPlay(0);
	}
	else if (type == 1)
	{
		GameManager::instance()->soundplayer.ItemPlay(1);
		if (HeavyMachineGun)
			GameManager::instance()->nBullet += 150;
		else
		{
			HeavyMachineGun = true;
			GameManager::instance()->nBullet = 200;
		}
	}
	else if(type == 2)
	{
		parent->camera->PhaseClear();
	}
}

void Player::Resurrection()
{
	active = true;
	Transition(RESURRECTION);
	invincible = GetTickCount();
	HeavyMachineGun = false;
	GameManager::instance()->nBullet = 0;
	GameManager::instance()->nBomb = 10;
}
