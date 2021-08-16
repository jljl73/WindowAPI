#include "Camera.h"

Camera::Camera()
{
	GameManager::instance()->phase = 0;
	zoom = GameManager::instance()->zoom;
	maxX = 3962 * zoom - GameManager::instance()->screenWidth;
	phaseX.push_back(868);
	phaseX.push_back(1310);
	phaseX.push_back(1910);
	phaseX.push_back(2340);
	phaseX.push_back(3220);
	phaseX.push_back(3520);
	//phaseX.push_back(maxX);
}

void Camera::Update()
{
	transform = transform + velocity;
	GameManager::instance()->a = transform;
}

void Camera::MoveX(int x)
{
	if (transform.X < phaseX[GameManager::instance()->phase] * zoom)
		velocity.X = x;
	else
		velocity.X = 0;
}

void Camera::MoveY(int y)
{
	transform.Y += y;
}

bool Camera::EndPhase()
{
	return transform.X >= phaseX[GameManager::instance()->phase] * zoom;
}

bool Camera::isOut(GameObjects* object)
{
	if (object->Position().X < transform.X || object->Position().X > RightX() ||
		object->Position().Y < 0)
		return true;
	return false;
}

bool Camera::isLeftOut(GameObjects * object)
{
	if (object->Position().X < transform.X - object->getW()*GameManager::instance()->zoom)
		return true;
	return false;
}
