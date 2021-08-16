#pragma once
#include "Gutil.h"
#include <fstream>

class DataAni
{
public:
	int x;
	int y;
	int w;
	int h;
	int offset_x;
	int offset_y;
	DataAni(int _x, int _y, int _w, int _h, int _offset_x, int _offset_y)
		: x(_x), y(_y), w(_w), h(_h), offset_x(_offset_x), offset_y(_offset_y) {}
};

class Animation
{
	Image *pImg;
	Color color1, color2;
	int zoom;
	int maxFrame = 0;
	int maxCount = 1;
	int initial;
	int ct;
	int curFrame;
	int w;
	int h;
	vector<DataAni> vFrame;

public:
	void Init(Image * _pImg, Color _color1, Color _color2, int _i, int _maxFrame, int _maxCount,
		const char * path);

	int Play(Graphics& graphics, Point pos, bool left);
	int Print(Graphics& graphics, Point pos, bool left, int frame);
	int Play(Graphics& graphics, Point pos, bool left, bool hit);
	int Print(Graphics& graphics, Point pos, bool left, int frame, bool hit);
	int Play(Graphics& graphics, Point pos, bool left, bool hit, int repeat);

	void reset() {
		curFrame = 0; ct = 0;
	}
	int NextFrame();
	int CurFrame() { return curFrame; }
};

