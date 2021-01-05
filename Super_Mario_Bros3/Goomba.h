#pragma once
#include "GameObject.h"
#include "Rectangle.h"
#include "Pipe.h"
#include "Brick.h"

#define GOOMBA_WALKING_SPEED 0.05f
#define GOOMBA_JUMP_SPEED		0.35f
#define GOOMBA_TIME_JUMPING		900

#define GOOMBA_NORMAL_BBOX_WIDTH 16
#define GOOMBA_NORMAL_BBOX_HEIGHT 16
#define GOOMBA_BBOX_HEIGHT_DIE 9
#define GOOMBA_BBOX_FLY_WIDTH	19
#define GOOMBA_BBOX_FLY_HEIGHT_EAR_DOWN 20
#define GOOMBA_BBOX_FLY_HEIGHT_EAR_UP 24

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define	GOOMBA_STATE_DISAPPEAR	300
#define GOOMBA_STATE_DIE_BY_KICK	400
#define GOOMBA_STATE_GEARING 500
#define GOOMBA_STATE_FLYING 600

#define GOOMBA_ANI_ORANGE_WALKING 0
#define GOOMBA_ANI_ORANGE_DIE 1
#define GOOMBA_ANI_RED_FLY_WALKING 2
#define GOOMBA_ANI_RED_DIE 3
#define GOOMBA_ANI_RED_WALKING	4
#define GOOMBA_ANI_RED_GEARING 5
#define GOOMBA_ANI_RED_FLYING	6

#define	GOOMBA_DYING_TIME 300
#define GOOMBA_TIME_WALKING 300
#define GOOMBA_TIME_GEARING 600
#define GOOMBA_TIME_FLYING 900

#define	GOOMBA_TYPE_ORANGE 1
#define	GOOMBA_TYPE_RED_FLY 2
#define	GOOMBA_TYPE_RED_WALK 3
#define GOOMBA_DIE_DEFLECT_SPEED 0.5f

#define GOOMBA_JUMP_DEFLECT_SPEED 0.35f
class CGoomba : public CGameObject
{

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	int dying;
	DWORD dying_start = 0;
	int Type;
	int jumping_start = 0;
public:
	CGoomba(int type);
	virtual void SetState(int state);
	void StartDyingTime() { dying_start = GetTickCount(); }
	int GetType() { return Type; };
	void SetType(int type) { this->Type= type; };
};