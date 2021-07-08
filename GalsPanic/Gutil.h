#pragma once
#include <ObjIdl.h>
#include <gdiplus.h>
#include <vector>

using namespace std;
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")


int ccw(Point p1, Point p2, Point p3);
int onEdge(Point p, vector<Point> &polygon);
bool onScreen(Point p, RECT rt);
bool isSame(Point p1, Point p2);
bool isIntersection(Point a1, Point a2, Point b1, Point b2);
bool isInPolygon(Point p, vector<Point> &polygon);
int getArea(const vector<Point> &polygon);
int AddPoint(vector<Point> &polygon, vector<Point>& newPoint, int &, int &);

Point getIntersect(Point a1, Point a2, vector<Point> &polygon);