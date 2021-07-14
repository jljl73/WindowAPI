#include "Map.h"
#include <algorithm>
// 숨겨진 그림의 전부
void Map::DrawAll(Graphics & graphics, const WCHAR* p)
{
	Image *pImg = nullptr;
	pImg = Image::FromFile(p);
	if (!pImg)
		return;

	int w = pImg->GetWidth();
	int h = pImg->GetHeight();
	graphics.DrawImage(pImg, 0, 0, w, h);

	if (pImg)
		delete pImg;
}

// 공개된 공간을 제외하고 전부 가림
void Map::DrawOpen(Graphics & graphics, RECT &screenRect, const WCHAR* p)
{
	Image *pImg = nullptr;
	pImg = Image::FromFile(p);
	if (!pImg)
		return;

	GraphicsPath path;

	ImageAttributes imgAttr;
	SolidBrush brush(Color(255, 0, 0, 105));
	Rect rt(0, 0, screenRect.right, screenRect.bottom);

	int n = points.size();
	Point *polygons = new Point[n];

	for (int i = 0; i < n; ++i)
		polygons[i] = points[i];
	
	path.AddRectangle(rt);
	path.AddPolygon(polygons, n);
	graphics.FillPath(&brush, &path);

	if (1)
	{
		Pen pen(Color(255, 0, 180, 0));
		GraphicsPath path;

		int i = 0;
		for (i = 0; i < n - 1; ++i)
			graphics.DrawLine(&pen, points[i], points[i + 1]);
		if (i)
			graphics.DrawLine(&pen, points[i], points[0]);
	}

	delete[] polygons;
	if (pImg)
		delete pImg;
}

void Map::DeleteDuplicate()
{
	for (int i = 1; i < points.size(); ++i)
	{
		if (isSame(points[i], points[i - 1]))
		{
			points.erase(points.begin() + i--);
		}
	}
	if (isSame(points[points.size()-1], points[0]))
	{
		points.erase(points.begin());
	}
}

bool Map::push_back(Point _p)
{
	points.push_back(_p);
	return true;
}

int Map::OnEdge(Point &p) // 해당하는 선 리턴
{
	int n = points.size();
	
	for (int i = 1; i < n; ++i)
	{
		if (p.X == points[i - 1].X) // 모두 직선이다는 가정
		{
			if (min(points[i].Y, points[i - 1].Y) <= p.Y &&
				max(points[i].Y, points[i - 1].Y) >= p.Y)
				return i-1;
		}
		else if (p.Y == points[i-1].Y)
		{
			if (min(points[i].X, points[i - 1].X) <= p.X &&
				max(points[i].X, points[i - 1].X) >= p.X)
				return i-1;
		}
	}

	if (p.X == points[n-1].X) // 마지막하고 처음도 체크
	{
		if (min(points[n-1].Y, points[0].Y) <= p.Y &&
			max(points[n-1].Y, points[0].Y) >= p.Y)
			return n-1;
	}
	else if(p.Y == points[n-1].Y)
	{
		if (min(points[n-1].X, points[0].X) <= p.X &&
			max(points[n-1].X, points[0].X) >= p.X)
			return n-1;
	}
	
	return -1;
}

