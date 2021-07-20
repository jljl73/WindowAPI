#pragma once
#include "framework.h"
#include <ObjIdl.h>
#include <gdiplus.h>
#include "Player.h"
#include "Map.h"
#include "Enemy.h"
#include "UI.h"
#include "Sound.h"
#include <memory>

using namespace std;
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

enum gState { START, SELECT, INGAME, CLEAR, OVER, END };

class GameManager
{
	ULONG_PTR					g_GdiToken;
	HBITMAP						hDoubleBufferImage;
	HWND						hWnd;

	unique_ptr<Player>			player;
	vector<unique_ptr<Enemy>>	enemies;
	vector<const WCHAR*>		path;
	Map							map;

	UI							ui;
	Sound						bgm, effect, effect2;

	int							score;
	int							state;
	int							select;
	DWORD						oldTime;
	int							gtime;

public:
	GameManager();
	RECT						screenRect;

	void						Init(HWND);
	void						GameReset();
	void						Update();
	void						DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc);
	void						Transition(int _state);

	void						Select(int key);
	void						GDI_Init();
	void						GDI_Draw(HDC hdc);
	void						GDI_End();

	int  getState() { return state; }
	void DrawStartScreen(Graphics& graphics);
	void DrawSelectScreen(Graphics& graphics);
	void DrawGameScreen(Graphics& graphics);
	void DrawClearScreen(Graphics& graphics);
	void DrawOverScreen(Graphics& graphics);
	void DrawEndScreen(Graphics& graphics);
};