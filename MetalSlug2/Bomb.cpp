#include "Bomb.h"
#include "ScenePlay.h"
Bomb::Bomb(Scene *_scene, Point p, Point v)
{
	parent = (ScenePlay*)_scene;

	transform = p;
	velocity = v;
	ani[0].Init(parent->boImg, Color(0, 247, 0), Color(1, 249, 1), 0, 16, 2,
		"data/Animation/etc/bomb.txt");
	ani[1].Init(parent->exploisionImg, Color(0, 247, 0), Color(1, 249, 1), 0, 25, 0,
		"data/Animation/etc/Bomb_explosion.txt");
}

void Bomb::Update()
{
	if (!isActive())
		return;

	velocity.Y += GameManager::instance()->zoom;;
	bool onGround = OnGravity(transform, velocity, parent->stage1->ground, 20)
	|| OnGravity(transform, velocity, parent->stage1->upperGround, 20);

	if (onGround)
	{
		if (bounce)
			Transition(1);
		velocity.Y -= 5 * GameManager::instance()->zoom;
		bounce = true;
	}

	transform = transform + velocity;
}

void Bomb::Draw(Graphics &graphics)
{
	int zoom = GameManager::instance()->zoom;
	Point position(transform.X - parent->camera->position().X, transform.Y - parent->camera->position().Y);

	if(state == 0)
		ani[0].Play(graphics, position, dir < 0);
	else
	{
		if (ani[1].Play(graphics, position, dir < 0))
			Transition(-1);
	}

}

void Bomb::Transition(int _state)
{
	GameObjects::Transition(_state);
	if (state == 1)
	{
		active = false;
		GameManager::instance()->soundplayer.EffectPlay(1);
	}
}
