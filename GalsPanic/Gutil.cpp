#include "Gutil.h"

int ccw(Point p1, Point p2, Point p3)
{
	int a = p1.X * p2.Y + p2.X * p3.Y + p3.X * p1.Y;
	int b = p1.Y * p2.X + p2.Y * p3.X + p3.Y * p1.X;

	return a - b;
}

int onEdge(Point p, vector<Point>& polygon)
{
	int n = polygon.size();
	if (!n)return -1;


	for (int i = 1; i < n; ++i)
	{
		if (p.X == polygon[i - 1].X) // 모두 직선이다는 가정
		{
			if (min(polygon[i].Y, polygon[i - 1].Y) <= p.Y &&
				max(polygon[i].Y, polygon[i - 1].Y) >= p.Y)
				return i - 1;
		}
		else if (p.Y == polygon[i - 1].Y)
		{
			if (min(polygon[i].X, polygon[i - 1].X) <= p.X &&
				max(polygon[i].X, polygon[i - 1].X) >= p.X)
				return i - 1;
		}
	}


	return -1;
}

bool onScreen(Point p, RECT rt)
{
	if (p.X <= 0 || p.X >= rt.right || p.Y <= 0 || p.Y >= rt.bottom)
		return false;
	return true;
}

bool isSame(Point p1, Point p2)
{
	if ((p1.X == p2.X) && (p1.Y == p2.Y))
		return true;
	return false;
}

// 두 선분이 교차하는가
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

bool isParellel(Point a1, Point a2, Point b1, Point b2)
{


	return true;
}

// 점이 다각형 내부에 있는가
bool isInPolygon(Point p, vector<Point>& polygon)
{
	Point origin(1, 0);
	
	int ct = 0;
	int n = polygon.size();
	int nct = 0;

	for (int i = 1; i < n; i++)
	{
		if (isIntersection(origin, p, polygon[i - 1], polygon[i]))
		{
			++ct;
			if (ccw(origin, p, polygon[i]) == 0 &&
				isIntersection(origin, p, polygon[(i - 1) % n], polygon[(i + 1) % n]))
			{
				++nct;
			}
		}

	}

	if (isIntersection(origin, p, polygon[0], polygon[n - 1]))
	{
		++ct;
		if (ccw(origin, p, polygon[0]) == 0 &&
			isIntersection(origin, p, polygon[n - 1], polygon[1]))
			++nct;
	}

	ct = ct - nct;

	if ((ct & 1) == 1)
		return true;
	else
		return false;
}

int getArea(const vector<Point>& polygon)
{
	Point origin = polygon[0];
	int n = polygon.size();
	int area = 0;
	for (int i = 1; i < n - 1; ++i)
	{
		area += ccw(origin, polygon[i], polygon[i + 1]);
	}

	return area >> 1;
}

int AddPoint(vector<Point>& mapPoints, vector<Point>& newPoints, 
	int& start, int& end)
{
	if (newPoints.size() < 2)
	{
		newPoints.clear();
		return 0;
	}

	int area = getArea(newPoints);
	if (start > end) // 한바퀴 돌았을떄
	{
		mapPoints.erase(mapPoints.begin() + start + 1, mapPoints.end());
		mapPoints.erase(mapPoints.begin(), mapPoints.begin() + end + 1);
		mapPoints.insert(mapPoints.end(), newPoints.begin(), newPoints.end());

	}
	else
	{
		int t = 0;
		Point temp = mapPoints[0];
		while (isSame(temp, newPoints.front()) || isSame(temp, newPoints.back()))
			temp = mapPoints[++t];

		// 한바퀴 돌아서 다시 올때
		if ((start == end) && isInPolygon(temp, newPoints))
		{
			mapPoints.clear();
			mapPoints.insert(mapPoints.begin(), newPoints.begin(), newPoints.end());
		}
		else
		{
			for (int i = 0; i < end - start; ++i)
				mapPoints.erase(mapPoints.begin() + start + 1);

			mapPoints.insert(mapPoints.begin() + start + 1, newPoints.begin(), newPoints.end());
		}
	}

	newPoints.clear();
	return getArea(mapPoints);
}


Point getIntersect(Point a1, Point a2, vector<Point>& polygon)
{
	Point it(-1, -1);

	int n = polygon.size();
	for (int i = 1; i < n; i++)
	{
		if (isIntersection(a1, a2, polygon[i - 1], polygon[i]))
		{
			// 직선일때만
			if (a1.X == a2.X)
			{
				it.X = a1.X;
				it.Y = polygon[i].Y;
			}
			else
			{
				it.X = polygon[i].X;
				it.Y = a1.Y;
			}
			return it;
		}
	}

	if (isIntersection(a1, a2, polygon[n-1], polygon[0]))
	{
		// 직선일때만
		if (a1.X == a2.X)
		{
			it.X = a1.X;
			it.Y = polygon[0].Y;
		}
		else
		{
			it.X = polygon[0].X;
			it.Y = a1.Y;
		}
	}

	return it;
}
