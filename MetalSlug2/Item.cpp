#include "Item.h"
#include "ScenePlay.h"
Item::Item(Scene * _scene, Point pos, int _type)
{
	parent = (ScenePlay*)_scene;

	if (parent->itemImg == NULL)
		parent->itemImg = Image::FromFile(_T("data/Sprite/Items.png"));

	ani = new Animation();
	transform = pos;
	type = _type;
	
	switch (type)
	{
	case BOMBBOX:
		ani->Init(parent->itemImg, Color(255, 255, 255), Color(255, 255, 255), 0, 6,
			100, "data/Animation/etc/BombBox.txt");
		break;
	case H:
		ani->Init(parent->itemImg, Color(255, 255, 255), Color(255, 255, 255), 0, 1,
			100, "data/Animation/etc/H.txt");
		break;
	case 2:
		w = 987654321;
		h = 987654321;
		break;
	default:
		break;
	}
}

void Item::Draw(Graphics &graphics)
{
	Point position(transform.X - parent->camera->position().X, transform.Y - parent->camera->position().Y);
	ani->Play(graphics, position, dir < 0);
}

void Item::Update()
{
	velocity.Y += 2;
	bool onGround = OnGravity(transform, velocity, parent->stage1->ground, 20)
		|| OnGravity(transform, velocity, parent->stage1->upperGround, 20);
	transform = transform + velocity;
}
