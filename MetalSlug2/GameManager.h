#pragma once
#include "Gutil.h"
#include "Scene.h"

class GameManager
{
	enum STATEIDX { MAIN, SELECT, PLAY, END};

	ULONG_PTR g_GdiToken;
	HBITMAP hDoubleBufferImage;
	
	
	// ΩÃ±€≈Ê

	static GameManager* _instance;
	static bool instanceFlag;

	GameManager();

public:
	HWND hWnd;
	Scene *scenePlay;
	int screenHeight;
	int screenWidth;
	const int zoom = 3;

	// Player
	int score;
	int nBomb;
	int nBullet;
	int nHP;
	int sState;

	SoundPlayer soundplayer;
	int phase;
	bool clear;
	Point a;

	static GameManager* instance();

	// GDI
	void GDI_Init();
	void GDI_Draw(HDC);
	void GDI_End();
	void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc);

	//
	void Transition(int dest);
	void Update();
};