#pragma once
#include "Gutil.h"
#include <vector>

using namespace std;

class Map
{
	//Point *polygons;
public:
	vector<Point> points;
	Map() 
	{ 
	}
	~Map() {
	}

	void DrawAll(Graphics &graphics, const WCHAR* p);
	void DrawOpen(Graphics & graphics, RECT &screenRect, const WCHAR* p);
	void DeleteDuplicate();
	bool push_back(Point);
	int OnEdge(Point&);
};