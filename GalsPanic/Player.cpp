#include "Player.h"

void Player::ChangeState(int _state)
{
	state = _state;
}

void Player::ChangeSpace(bool _onSpace)
{
	onSpace = _onSpace;
}

void Player::Draw(Graphics &graphics)
{
	// 캐릭터 이미지
	ImageAttributes imgAttr;
	imgAttr.SetColorKey(Color(255, 116, 110), Color(255, 116, 111));
	pImg = Image::FromFile((WCHAR*)L"img/Player.png");
	if (!pImg)
		return;

	int standard_x = 12;
	int standard_y = 216;
	int w = 38;
	int h = 38;

	if (state == HIT)
	{
		standard_x = 116;
		standard_y = 252;

		graphics.DrawImage(pImg, Rect(hitPos.X - w / 2, hitPos.Y - h / 2, w, h),
			standard_x + w * (frame/2), standard_y,
			w, h,
			UnitPixel, &imgAttr);
	}
	else 
	{
		standard_x = 12;
		standard_y = 216;

		graphics.DrawImage(pImg, Rect(pos.X - w / 2, pos.Y - h / 2, w, h),
			standard_x + w * frame, standard_y,
			w, h,
			UnitPixel, &imgAttr);
	}

	if(pImg)
		delete[] pImg;

	// 지나온 선분 그리기
	if (1)
	{
		Pen pen(Color(255, 255, 0, 0));
		GraphicsPath path;
		int n = newPoints.size();

		int i = 0;
		for(i = 0; i < n-1; ++i)
			graphics.DrawLine(&pen, newPoints[i], newPoints[i+1]);
		if(n)
			graphics.DrawLine(&pen, newPoints[i], pos);
	}
}

void Player::Move(Map &map, int _d)
{
	if (state == HIT)
		return;

	Point tP(pos);
	
	int nextPointLine;
	int curPointLine;
	switch (_d)
	{
	case VK_LEFT:
		tP.X -= speed;
		break;
	case VK_RIGHT:
		tP.X += speed;
		break;
	case VK_UP:
		tP.Y -= speed;
		break;
	case VK_DOWN:
		tP.Y += speed;
		break;
	default:
		break;
	}
	//tP.X -= speed;


	if (!onScreen(tP, screenRect) || onEdge(tP, newPoints) > -1)
		return;

	nextPointLine = map.OnEdge(tP);
	if (nextPointLine == -1 && isInPolygon(tP, map.points))
	{
		return;
	}

	curPointLine = map.OnEdge(pos);
	//temp = map.OnEdge(tP);

	Point p = { (pos.X + tP.X) / 2, (pos.Y + tP.Y) / 2 };
	int a = isInPolygon(p, map.points);
	int b = map.OnEdge(p);

	if (nextPointLine > -1) // 다음 위치가 Edge인 경우
	{
		//lineIdx = nextPointLine;
		if (onSpace && curPointLine == -1)
		{
			newPoints.push_back(pos);
			newPoints.push_back(tP);
			EatArea(map, nextPointLine);
		}
		else if (onSpace && !a)
		{
			lineIdx = curPointLine;
			newPoints.push_back(pos);
			newPoints.push_back(pos);
			newPoints.push_back(tP);
			EatArea(map, nextPointLine);
		}
		else if (curPointLine > -1)
		{
			newPoints.clear();
			if (b == -1)
				return;
		}
	
		pos = tP;
		ChangeState(EDGE);
		return;
	}
	else if(curPointLine > -1) // 현재 위치가 Edge인 경우
	{
		lineIdx = curPointLine;
		newPoints.push_back(pos);
	}

 	ChangeState(FIELD);
	if (onSpace)
	{
		if(dir != _d)
			newPoints.push_back(pos);

		if(onEdge(tP, newPoints) == -1)// 바로 뒤로 다시가는거 방지
			pos = tP;
	}

	dir = _d;
}

void Player::BackTrack()
{
	if (state == FIELD && !onSpace && !newPoints.empty())
	{
		Point tp = newPoints.back();
		if(isSame(tp, pos))
			newPoints.erase(newPoints.end() - 1);
		else
		{
			Point unit;
			unit.X = tp.X - pos.X;
			unit.Y = tp.Y - pos.Y;
			if (unit.X != 0)
			{
				unit.X = unit.X * speed / abs(unit.X);
				pos.X += unit.X;
			}
			if (unit.Y != 0)
			{
				unit.Y = unit.Y * speed / abs(unit.Y);
				pos.Y += unit.Y;
			}

		}

		if (newPoints.empty())
			ChangeState(EDGE);
	}
}


void Player::EatArea(Map &map, int end)
{
	//if (newPoints.size() < 2)
	//{
	//	newPoints.clear();
	//	return;
	//}

	int area = getArea(newPoints);

	vector<Point> mapPoints2(map.points);
	vector<Point> newPoints2(newPoints);
	reverse(newPoints2.begin(), newPoints2.end());

	int area1 = AddPoint(map.points, newPoints, lineIdx, end);
	int area2 = AddPoint(mapPoints2, newPoints2, end, lineIdx);

	if (area2 > area1)
		map.points = mapPoints2;

	//map.DeleteDuplicate();
}

void Player::NextFrame()
{
	if (state == HIT && ++frame > maxframe)
	{
		state = EDGE;
	}

	if (state != HIT && ++frame > maxframe)
		frame = 0;
}

void Player::Hit()
{
	--hp;
	hitPos = pos;
	pos = newPoints[0];
	newPoints.clear();
	state = HIT;
	frame = 0;
}
