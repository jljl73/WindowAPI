#pragma once
#include "framework.h"
#include <ObjIdl.h>
#include <gdiplus.h>
#include <vector>
#include <memory>
using namespace std;
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

class Line
{
public:
	Point src;
	Point dest;
	Line(Point _s, Point _d) { src = _s; dest = _d; }
};

bool OnGravity(Point & position, Point &velocity, vector<Line> &ground, int size);
bool OnWall(Point &position, Point &velocity, vector<Line> &ground, int size);
int ccw(Point p1, Point p2, Point p3);
bool isIntersection(Point a1, Point a2, Point b1, Point b2);
int distance(Point a, Point b);