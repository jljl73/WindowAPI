#pragma once
#include "GameObjects.h"
#include "Animation.h"
class ScenePlay;

class NPC : public GameObjects
{
	enum NPCSTATE {
		DESTROYED = -1, IDLE, PATROL, DROPITEM, SALUTE, RUNAWAY };

	ScenePlay *parent;
	Animation *ani;
	int dropItemType;
	bool saved;
public:
	NPC(ScenePlay *_scene, Point pos, int type, int _dir);
	virtual void Init();
	virtual void Update();
	virtual void Draw(Graphics &);
	virtual void Transition(int _state);
	void DropItem();
	void Saved();
	bool isSaved();
};

