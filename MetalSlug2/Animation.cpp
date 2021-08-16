#include "Animation.h"
#include "GameManager.h"
void Animation::Init(Image * _pImg, Color _color1, Color _color2, int _i, int _maxFrame, int _maxCount,
	const char * path)
{
	ifstream ifs;
	ifs.open(path);

	zoom = GameManager::instance()->zoom;
	pImg = _pImg;
	curFrame = 0; 
	ct = 0;
	initial = _i;
	color1 = _color1;
	color2 = _color2;
	maxFrame = _maxFrame;
	maxCount = _maxCount;

	int x, y, w, h, offset_x, offset_y;
	for (int i = 0; i < maxFrame; ++i)
	{
		ifs >> x >> y >> w >> h >> offset_x >> offset_y;
		vFrame.push_back(DataAni(x, y, w, h, offset_x, offset_y));
	}
	ifs.close();
}

int Animation::Play(Graphics& graphics, Point pos, bool bLeft)
{
	Print(graphics, pos, bLeft, curFrame);
	return NextFrame();
}

int Animation::Print(Graphics & graphics, Point pos, bool bLeft, int frame)
{
	ImageAttributes imgAttr;
	imgAttr.SetColorKey(color1, color2);

	if (frame >= vFrame.size())
		return 0;

	Matrix mat;
	int dir = 1;
	if (bLeft)
		dir = -1;
	mat.SetElements(dir, 0, 0, 1, 0, 0);
	graphics.SetTransform(&mat); 


	graphics.DrawImage(pImg,
		Rect(dir*(pos.X + dir * ((vFrame[frame].offset_x)) * zoom),
			pos.Y + (vFrame[frame].offset_y - vFrame[frame].h) * zoom
			, vFrame[frame].w*zoom, vFrame[frame].h*zoom),
		vFrame[frame].x, vFrame[frame].y,
		vFrame[frame].w, vFrame[frame].h,
		UnitPixel, &imgAttr);

	return 0;
}

int Animation::Play(Graphics & graphics, Point pos, bool bLeft, bool hit)
{
	Print(graphics, pos, bLeft, curFrame, hit);
	return NextFrame();
}

int Animation::Print(Graphics & graphics, Point pos, bool bLeft, int frame, bool hit)
{
	ImageAttributes imgAttr;
	imgAttr.SetColorKey(color1, color2);

	if (hit)
	{
		ColorMatrix colorMat = {
				2, 0, 0, 0, 0,
				0, 1, 0, 0, 0,
				0, 0, 1, 0, 0,
				0, 0, 0, 1, 0,
				0.2, 0.2, 0.2, 0, 1
		};
		imgAttr.SetColorMatrix(&colorMat);
	}
	if (frame >= vFrame.size())
		return 0;

	Matrix mat;
	int dir = 1;
	if (bLeft)
		dir = -1;
	mat.SetElements(dir, 0, 0, 1, 0, 0);
	graphics.SetTransform(&mat);


	graphics.DrawImage(pImg,
		Rect(dir*(pos.X + dir * ((vFrame[frame].offset_x)) * zoom),
			pos.Y + (vFrame[frame].offset_y - vFrame[frame].h) * zoom
			, vFrame[frame].w*zoom, vFrame[frame].h*zoom),
		vFrame[frame].x, vFrame[frame].y,
		vFrame[frame].w, vFrame[frame].h,
		UnitPixel, &imgAttr);

	return 0;
}

int Animation::Play(Graphics & graphics, Point pos, bool left, bool hit, int repeat)
{
	static int ct = 0;
	ct += Play(graphics, pos, left, hit);

	if (ct == repeat)
	{
		ct = 0;
		return 1;
	}
	else
		return 0;
}

int Animation::NextFrame()
{
	++ct;
	if (ct > maxCount)
	{
		ct = 0;
		if (++curFrame >= maxFrame)
		{
			curFrame = initial;
			return 1;
		}
	}
	return 0;
}
