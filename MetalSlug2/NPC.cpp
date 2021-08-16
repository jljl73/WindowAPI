#include "NPC.h"
#include "ScenePlay.h"

NPC::NPC(ScenePlay * _scene, Point pos, int type, int _dir)
{
	w = 20;
	h = 40;
	parent = _scene;
	transform = pos;
	dropItemType = type;
	ani = new Animation[5];
	dir = _dir;
	saved = false;
}

void NPC::Init()
{
	if (parent->npcImg == NULL)
		parent->npcImg = Image::FromFile(_T("data/Sprite/NPC.png"));

	ani[0].Init(parent->npcImg, Color(255, 255, 255), Color(255, 255, 255), 0, 2,
		3, "data/Animation/NPC/Idle.txt");
	ani[1].Init(parent->npcImg, Color(255, 255, 255), Color(255, 255, 255), 8, 19,
		1, "data/Animation/NPC/Patrol.txt");
	ani[2].Init(parent->npcImg, Color(255, 255, 255), Color(255, 255, 255), 0, 11,
		1, "data/Animation/NPC/DropItem.txt");
	ani[3].Init(parent->npcImg, Color(255, 255, 255), Color(255, 255, 255), 0, 14,
		1, "data/Animation/NPC/Salute.txt");
	ani[4].Init(parent->npcImg, Color(255, 255, 255), Color(255, 255, 255), 0, 8,
		1, "data/Animation/NPC/RunAway.txt");
}

void NPC::Update()
{
	velocity.Y += 2;
	OnGravity(transform, velocity, parent->stage1->ground, 20);
	OnGravity(transform, velocity, parent->stage1->upperGround, 20);

	if (state == PATROL && ani[1].CurFrame() > 7)
	{
		velocity.X = GameManager::instance()->zoom * dir;
	}
	else if (state == RUNAWAY)
	{
		velocity.X = GameManager::instance()->zoom * 10 * dir;
	}
	else
		velocity.X = 0;


	transform = transform + velocity;
}

void NPC::Draw(Graphics &graphics)
{
	Pen pen(Color(255, 0, 0));
	Point position(transform.X - parent->camera->position().X, transform.Y - parent->camera->position().Y);
	/*graphics.DrawRectangle(&pen, Rect(position.X - (w >> 1)*GameManager::instance()->zoom, position.Y - (h)*GameManager::instance()->zoom,
		w *GameManager::instance()->zoom, h *GameManager::instance()->zoom));*/

	switch (state)
	{
	case 0:
		ani[0].Play(graphics, position, dir > 0);
		break;
	case 1:
		if (ani[1].Play(graphics, position, dir > 0))
			dir *= -1;
		break;
	case 2:
		if (ani[2].Play(graphics, position, dir > 0))
		{
			DropItem();
			Transition(3);
		}
		break;
	case 3:
		if (ani[3].Play(graphics, position, dir > 0))
		{
			Transition(4);
		}
		break;
	case 4:
		ani[4].Play(graphics, position, dir > 0);
		break;
	}
}

void NPC::Transition(int _state)
{
	state = _state;
	if (state == 2)
	{
		GameManager::instance()->soundplayer.ItemPlay(2);
		active = false;
	}
}

void NPC::DropItem()
{
	if (dropItemType > -1)
	{
		Point p(transform.X + dir * w * GameManager::instance()->zoom, transform.Y);
		parent->items.push_back(new Item(parent, p, dropItemType));
	}
	active = false;
}

void NPC::Saved()
{
	Transition(PATROL);
	saved = true;
}
bool NPC::isSaved()
{
	return saved;
}