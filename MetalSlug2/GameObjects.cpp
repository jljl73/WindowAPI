#include "GameObjects.h"
#include "GameManager.h"

bool Collision(GameObjects * p, GameObjects * q)
{
	if (!p->isActive() || !q->isActive())
		return false;

	Point diff(p->transform.X - q->transform.X,
		(p->transform.Y - (p->h >>1)) - (q->transform.Y - (q->h >> 1)));

	Point dist(p->w + q->w, p->h + q->h);

	if (abs(diff.X) <= abs(dist.X) && abs(diff.Y) <= abs(dist.Y))
		return true;
	return false;
}

bool GameObjects::isCollision(Point q)
{
	if (!this->isActive())
		return false;

	Point diff(this->transform.X - q.X, (this->transform.Y - (this->h >> 1)) - q.Y);

	Point dist(this->w * GameManager::instance()->zoom, this->h* GameManager::instance()->zoom);
	dist.X = dist.X >> 1;
	dist.Y = dist.Y >> 1;

	if (abs(diff.X) <= abs(dist.X) && abs(diff.Y) <= abs(dist.Y))
		return true;
	return false;
}
