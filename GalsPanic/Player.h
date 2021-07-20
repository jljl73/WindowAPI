#pragma once
#include "Gutil.h"
#include "Map.h"
#include "Enemy.h"
#include "Sound.h"
#include <memory>

enum pState { EDGE, FIELD, HIT };
class Player
{
	Point				pos;
	Image				*pImg;
	LONG				speed;
	RECT				screenRect; // 화면 크기
	int					lineIdx;//현재 위에있는 선의 인덱스
	int					frame, maxframe = 6;
	int					dir; // 움직이고 있는 방향
	bool				onSpace; // 스페이스를 누른상태인지
	
	Point				hitPos;
	int					state;
	int					hp;
	Sound				sound, sound2;
	HWND				hWnd;

public:
	vector<Point> newPoints;
	Player(Point _pos, RECT _rt, HWND _hWnd) : pos(_pos), onSpace(0), speed(10), pImg(0), dir(0), lineIdx(0), screenRect(_rt), hp(3), state(EDGE), frame(0)
	{
		hWnd = _hWnd;
		sound.Open(hWnd, TEXT("sounds/beep.mp3"));
		sound2.Open(hWnd, TEXT("sounds/death.mp3"));
	}
	~Player()
	{
		sound.Stop();
		sound2.Stop();
	}
	
	const Point getPos() { return pos; }
	int getHP() { return hp; }
	void ChangeState(int);
	void ChangeSpace(bool);
	void Draw(Graphics &graphics);
	void Move(Map & map, int);
	void BackTrack(); // 스페이스 뗏을때 돌아가기
	void EatArea(Map &map, int);
	void NextFrame();
	void Hit();
};