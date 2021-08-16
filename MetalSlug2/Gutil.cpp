#include "Gutil.h"
bool OnGravity(Point & position, Point & velocity, vector<Line>& ground, int size)
{
	int n = ground.size();
	Line* onGround = NULL;
	Point nextPosition = position + velocity;
	nextPosition.Y += size;

	for (int i = 0; i < n; ++i)
	{
		// 땅인지체크
		if (isIntersection(ground[i].src, ground[i].dest, position, nextPosition))
		{
			onGround = &ground[i];
			velocity.Y = 0;
			break;
		}
	}

	if (onGround != NULL)
	{
		position.Y = (onGround->dest.Y - onGround->src.Y)
			* (position.X - onGround->src.X)
			/ (onGround->dest.X - onGround->src.X)
			+ onGround->src.Y - size;
		return true;
	}
	return false;
}

bool OnWall(Point & position, Point & velocity, vector<Line>& ground, int size)
{
	int n = ground.size();
	Point nextPosition = position + velocity;

	for (int i = 0; i < n; ++i)
	{
		// 벽인지체크
		if (isIntersection(ground[i].src, ground[i].dest, position, nextPosition))
		{
			velocity.X = 0;
			break;
		}
	}

	return false;
}

int ccw(Point p1, Point p2, Point p3)
{
	int a = p1.X * p2.Y + p2.X * p3.Y + p3.X * p1.Y;
	int b = p1.Y * p2.X + p2.Y * p3.X + p3.Y * p1.X;

	return a - b;
}

bool isIntersection(Point a1, Point a2, Point b1, Point b2)
{
	if (b1.X == b2.X && b1.Y == b2.Y)
		return false;

	long long a = ccw(a1, a2, b1);
	long long b = ccw(a1, a2, b2);

	long long c = ccw(b1, b2, a1);
	long long d = ccw(b1, b2, a2);

	return ((a*b) <= 0 && (c*d) <= 0);
}

int distance(Point a, Point b)
{
	int x = a.X - b.X;
	int y = a.Y - b.Y;
	return (int)sqrt(x * x + y * y);
}
