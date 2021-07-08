#pragma once
#include "Gutil.h"
#include "Map.h"
#include "Enemy.h"
#include <memory>

enum pState { EDGE, FIELD };
class Player
{
	Point pos;
	Image *pImg;
	LONG speed;
	RECT screenRect; // ȭ�� ũ��
	int lineIdx;//���� �����ִ� ���� �ε���
	int frame;
	int dir; // �����̰� �ִ� ����
	bool onSpace; // �����̽��� ������������
	

	int state;
	int hp;

public:
	vector<Point> newPoints;
	Player(Point _pos, RECT _rt) : pos(_pos), onSpace(0), speed(10), pImg(0), dir(0), lineIdx(0), screenRect(_rt), hp(3), state(EDGE), frame(0)
	{
	}
	
	const Point getPos() { return pos; }
	int getHP() { return hp; }
	void ChangeState(int);
	void ChangeSpace(bool);
	void Draw(Graphics &graphics);
	void Move(Map & map, int);
	void BackTrack(); // �����̽� ������ ���ư���
	void EatArea(Map &map, int);
	void NextFrame();
	void Hit();
};

