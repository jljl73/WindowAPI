#include "Enemy.h"
#include <algorithm>
const double PI = acos(-1);

void Enemy::Draw(Graphics& graphics)
{
	pImg = Image::FromFile((WCHAR*)L"img/Enemy.png");
	if (!pImg)
		return;

	int standard_x = 36;
	int standard_y = 16;

	ImageAttributes imgAttr;
	imgAttr.SetColorKey(Color(255, 116, 110), Color(255, 116, 111));

	int w = 48;
	int h = 38;
	graphics.DrawImage(pImg, Rect(pos.X - w / 2, pos.Y - h / 2, w, h),
		standard_x + w * frame, standard_y,
		w, h,
		UnitPixel, &imgAttr);

	if (pImg)
		delete[] pImg;

}

void Enemy::NextFrame()
{
	++frame;
	if (frame > 6)
		frame = 0;
}

void Enemy::Move(Map &map, Player &player)
{
	Point nextPoint;
	nextPoint.X = pos.X + speed * cos(dir);
	nextPoint.Y = pos.Y + speed * sin(dir);

	if (!isInPolygon(nextPoint, map.points))
	{
		CheckCollisionWithNewMap(player, nextPoint);
		pos = nextPoint;
	}
	else
		RandomMove();
	if (rand() % 30 == 0)
		RandomMove();
}

void Enemy::RandomMove()
{
	dir = rand();
}

void Enemy::CheckCollisionWithScreen(RECT &screenRect)
{
	if (pos.X - radius < 0)
	{
		pos.X = radius;
		dir = PI - dir;
	}
	else if (pos.X + radius > screenRect.right)
	{
		pos.X = screenRect.right - radius;
		dir = PI - dir;
	}
	else if (pos.Y - radius < 0)
	{
		pos.Y = radius;
		dir = -dir;
	}
	else if (pos.Y + radius > screenRect.bottom)
	{
		pos.Y = screenRect.bottom - radius;
		dir = 2 * PI - dir;
	}
}

void Enemy::CheckCollisionWithMap(vector<Point>&polygon)
{
	int n = polygon.size();
	if (!n) return;
	for (int i = 1; i < n; ++i)
	{
		if (polygon[i].X == polygon[i - 1].X)
		{
			if (min(polygon[i].Y, polygon[i - 1].Y) <= pos.Y &&
				max(polygon[i].Y, polygon[i - 1].Y) >= pos.Y)
			{
				if (abs(polygon[i].X - pos.X) < radius)
				{
					dir += PI;
					if(polygon[i].X < pos.X)
						pos.X = polygon[i].X + radius + 1;
					else
						pos.X = polygon[i].X - radius - 1;
					return;
				}
			}
		}
		else
		{
			if (min(polygon[i].X, polygon[i - 1].X) <= pos.X &&
				max(polygon[i].X, polygon[i - 1].X) >= pos.X)
			{
				if (abs(polygon[i].Y - pos.Y) < radius)
				{
					dir += PI;
					if (polygon[i].Y < pos.Y)
						pos.Y = polygon[i].Y + radius + 1;
					else
						pos.Y = polygon[i].Y - radius - 1;
					return;
				}
			}
		}
	}


	if (polygon[0].X == polygon[n - 1].X)
	{
		if (min(polygon[0].Y, polygon[n - 1].Y) <= pos.Y &&
			max(polygon[0].Y, polygon[n - 1].Y) >= pos.Y)
		{
			if (abs(polygon[0].X - pos.X) < radius)
			{
				dir += PI;
				pos.X = polygon[0].X + radius;
				return;
			}
		}
	}
	else
	{
		if (min(polygon[0].X, polygon[n - 1].X) <= pos.X &&
			max(polygon[0].X, polygon[n - 1].X) >= pos.X)
		{
			if (abs(polygon[0].Y - pos.Y) < radius)
			{
				dir += PI;
				pos.Y = polygon[0].Y + radius;
				return;
			}
		}
	}

}

void Enemy::CheckCollisionWithNewMap(Player &player, Point &nextPoint)
{
	vector<Point> polygon(player.newPoints);
	int n = polygon.size();
	if (!n) return;

	for (int i = 1; i < n; ++i)
	{
		if (isIntersection(pos, nextPoint, polygon[i], polygon[i-1]))
		{
			player.Hit();
			return;
		}
	}

	if (isIntersection(pos, nextPoint, polygon[n-1], player.getPos()))
	{
		player.Hit();
	}

}
