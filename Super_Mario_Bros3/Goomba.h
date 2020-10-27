#pragma once
#include "GameObject.h"
#include "Rectangle.h"
#include "Pipe.h"
#include "Brick.h"

#define GOOMBA_WALKING_SPEED 0.05f;

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 15
#define GOOMBA_BBOX_HEIGHT_DIE 9

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define	GOOMBA_STATE_DISAPPEAR	300

#define GOOMBA_ANI_ORANGE_WALKING 0
#define GOOMBA_ANI_ORANGE_DIE 1
#define GOOMBA_ANI_RED_FLY_WALKING 2
#define GOOMBA_ANI_RED_FLY_DIE 3

#define	GOOMBA_DYING_TIME 1000

#define	GOOMBA_TYPE_ORANGE 1
#define	GOOMBA_TYPE_RED_FLY 2
class CGoomba : public CGameObject
{

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	int dying;
	DWORD dying_start;
	int Type;
public:
	CGoomba(int type);
	virtual void SetState(int state);
	void StartDyingTime() { dying_start = GetTickCount(); }
};