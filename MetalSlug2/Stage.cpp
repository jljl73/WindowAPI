#include "Stage.h"
#include "ScenePlay.h"
#include "GameManager.h"

Stage::Stage()
{
	int zoom = GameManager::instance()->zoom;
	ground.push_back(Line(Point(0, zoom * 196), Point(zoom * 148, zoom * 165)));
	ground.push_back(Line(Point(zoom * 148, zoom * 165), Point(zoom * 276, zoom * 196)));
	ground.push_back(Line(Point(zoom * 276, zoom * 196), Point(zoom * 330, zoom * 196)));
	ground.push_back(Line(Point(zoom * 330, zoom * 196), Point(zoom * 422, zoom * 181)));
	ground.push_back(Line(Point(zoom * 422, zoom * 181), Point(zoom * 495, zoom * 196)));
	ground.push_back(Line(Point(zoom * 495, zoom * 196), Point(zoom * 4000, zoom * 196)));

	upperGround.push_back(Line(Point(1706, 433 + 20), Point(1858, 433 + 20)));
	upperGround.push_back(Line(Point(1851, 330 + 20), Point(2041, 330 + 20)));
	upperGround.push_back(Line(Point(2121, 265 + 20), Point(2514, 265 + 20)));
	upperGround.push_back(Line(Point(3868, 289 + 20), Point(4383, 289 + 20)));
	//upperGround.push_back(Line(Point(4602, 468 + 20), Point(4803, 468 + 20)));
	upperGround.push_back(Line(Point(4739, 289 + 20), Point(5252, 289 + 20)));
	upperGround.push_back(Line(Point(6318, 243 + 20), Point(6636, 243 + 20)));
	upperGround.push_back(Line(Point(8462, 240 + 20), Point(9558, 240 + 20)));

	wall.push_back(Line(Point(4480, 0), Point(4480, 1000)));
}

Stage::~Stage()
{
	if (sImg)
		delete[] sImg;
}

void Stage::Init(Scene* _scene)
{
	parent = (ScenePlay*)_scene;
	sImg = Image::FromFile(_T("data/Sprite/Stages/stage1.png"));
	camera = (Camera*)parent->camera;
}

void Stage::Draw(Graphics &graphics)
{
	ImageAttributes imgAttr;
	imgAttr.SetColorKey(Color(248, 0, 248), Color(248, 0, 248));

	int width = GameManager::instance()->screenWidth;
	int height = GameManager::instance()->screenHeight;
	int zoom = GameManager::instance()->zoom;

	// 출력할 크기, 이미지의 위치, W,h, ?, 특징

	// BackGround
	graphics.DrawImage(sImg, Rect(0, 0, width, height*0.42), 1272, 510, 304, 96, UnitPixel, &imgAttr);
	graphics.DrawImage(sImg, Rect(0, height*0.21, width, height*0.48), 0, 512, 304, 96, UnitPixel, &imgAttr);
	graphics.DrawImage(sImg, Rect(0, 0, width, height),
		camera->position().X / zoom, (camera->position().Y / zoom) + 56, 304, 224, UnitPixel, &imgAttr);

	// 사과상자
	graphics.DrawImage(sImg, Rect(560 * zoom - camera->position().X, 13 * zoom - camera->position().Y, 144 * zoom, 197 * zoom),
		1467, 294, 144, 197, UnitPixel, &imgAttr);



	// 디버깅용 선
	/*Pen pen(Color(255, 0, 0));
	int n = ground.size();
	for (int i = 0; i < n; ++i)
		graphics.DrawLine(&pen, ground[i].src.X- camera->position().X, ground[i].src.Y - camera->position().Y,
			ground[i].dest.X - camera->position().X, ground[i].dest.Y - camera->position().Y);
	n = upperGround.size();
	for (int i = 0; i < n; ++i)
		graphics.DrawLine(&pen, upperGround[i].src.X - camera->position().X, upperGround[i].src.Y - camera->position().Y,
			upperGround[i].dest.X - camera->position().X, upperGround[i].dest.Y - camera->position().Y);

	n = wall.size();
	for (int i = 0; i < n; ++i)
		graphics.DrawLine(&pen, wall[i].src.X - camera->position().X, wall[i].src.Y - camera->position().Y,
			wall[i].dest.X - camera->position().X, wall[i].dest.Y - camera->position().Y);*/
}

void Stage::Update()
{
	transform.X = camera->position().X;
	transform.Y = camera->position().Y;
}