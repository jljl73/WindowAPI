#pragma once
#include "framework.h"
#include <ObjIdl.h>
#include <gdiplus.h>
#include "Player.h"
#include "Map.h"
#include "Enemy.h"
#include "UI.h"
#include <memory>
using namespace std;
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

enum gState { START, INGAME, CLEAR, OVER, END };

class GameManager
{

	ULONG_PTR					g_GdiToken;
	unique_ptr<Player>			player;
	vector<unique_ptr<Enemy>>	enemies;
	//unique_ptr<Enemy>			enemy;
	Map							map;
	UI							ui;
	HBITMAP						hDoubleBufferImage;
	int							score;
	int							state;
public:

	RECT						screenRect;

	void						Init();
	void						GameReset();
	void						Update();
	void						DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc);
	void						DestroyEnemy();
	void						Transition(int _state);
	void						GDI_Init();
	void						GDI_Draw(HDC hdc);
	void						GDI_End();

	int  getState() { return state; }
	void DrawStartScreen(Graphics& graphics);
	void DrawGameScreen(Graphics& graphics);
	void DrawClearScreen(Graphics& graphics);
	void DrawOverScreen(Graphics& graphics);
	void DrawEndScreen(Graphics& graphics);
};

class ScreenManager
{
	Image pImg;

public:
};