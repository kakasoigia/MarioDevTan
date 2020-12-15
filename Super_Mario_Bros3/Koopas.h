﻿#pragma once

#include "GameObject.h"
#include "Goomba.h"
#include "Mario.h"
#include "PlayScence.h"
#define KOOPAS_WALKING_SPEED 0.03f
#define KOOPAS_SPINNING_SPEED 0.3f

#define KOOPAS_TYPE_GREEN_WALK 1
#define KOOPAS_TYPE_GREEN_FLY 2
#define KOOPAS_TYPE_RED_WALK 3
#define KOOPAS_TYPE_RED_FLY 4

#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT 26
#define KOOPAS_BBOX_HEIGHT_DIE 16

#define KOOPAS_STATE_WALKING 100
#define KOOPAS_STATE_DIE 200
#define	KOOPAS_STATE_SHELL	300
#define	KOOPAS_STATE_SPINNING 400

//#define KOOPAS_ANI_GREEN_WALK_WALKING_LEFT 0
//#define KOOPAS_ANI_GREEN_WALK_WALKING_RIGHT 1
//#define	KOOPAS_ANI_GREEN_FLY_WALKING_LEFT 2
//#define	KOOPAS_ANI_GREEN_SHELL_PRONE 3
//#define	KOOPAS_ANI_GREEN_SHELL_UPWARD 4
//#define KOOPAS_ANI_RED_WALK_WALKING_LEFT 5
//#define KOOPAS_ANI_RED_FLY_WALKING_LEFT 6
//#define KOOPAS_ANI_RED_FLY_SHELL_UP 7

#define	KOOPAS_GRAVITY	0.002f


#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT 26
#define KOOPAS_BBOX_HEIGHT_SHELL 16
#define KOOPAS_BBOX_HEIGHT_DIE 16

#define KOOPAS_ANI_GREEN_WALKING_LEFT  0
#define KOOPAS_ANI_GREEN_WALKING_RIGHT 1
#define KOOPAS_ANI_GREEN_FLYING_LEFT   2

#define KOOPAS_ANI_GREEN_SHELL_PRONE	3
#define KOOPAS_ANI_GREEN_SHELL_UPWARD	  4	
#define KOOPAS_ANI_RED_WALKING_LEFT   5
#define KOOPAS_ANI_RED_WALKING_RIGHT	6
#define KOOPAS_ANI_RED_FLYING_LEFT	7
#define KOOPAS_ANI_RED_SHELL_PRONE	8
#define KOOPAS_ANI_GREEN_SPINNING  9
#define KOOPAS_ANI_RED_ANI_SPINNING	  10
#define KOOPAS_XANH_ANI_REVIVING	  11	
#define KOOPAS_RED_ANI_REVIVING		  12
#define KOOPAS_RED_MAI_ANI_NGUA		  13
#define KOOPAS_XANH_ANI_REVIVING_NGUA			14
#define KOOPAS_RED_ANI_REVIVING_NGUA			15
#define KOOPAS_BLACK_UP							16
#define KOOPAS_BLACK_NGUA						17
#define KOOPAS_XANH_ANI_WALKING_RIGHT_FASTER	18
#define KOOPAS_XANH_MAI_ANI_SPINNING_NGUA		19
#define KOOPAS_RED_MAI_ANI_SPINNING_NGUA		20

#define KOOPAS_DIE_DEFLECT_SPEED 0.75f
#define KOOPAS_FLY_DEFLECT_SPEED 0.4f
#define KOOPAS_PERIODIC_TIME_FLY 1000

#define KOOPAS_HOLDING 1
#define	KOOPAS_NOT_HOLDING 0

#define KOOPAS_TIME_REVIVE 5000
#define KOOPAS_TIME_ABOUT_TO_REVIVE 3000
class CKoopas : public CGameObject
{

	
	int Type;
	bool isHolding = false;
	int flying_start;
	bool reviveRender = false;
	bool canTurn = false;
	DWORD reviveStart= 0;
	bool CanPullBack = false;
	float CheckPosition_Y;

public:
	CKoopas(int type);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void FilterCollision(vector<LPCOLLISIONEVENT> &coEvents, vector<LPCOLLISIONEVENT> &coEventsResult, float &min_tx, float &min_ty, float &nx, float &ny, float &rdx, float &rdy);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void StartFlying() { flying_start = GetTickCount(); }
	virtual void SetState(int state);
	int GetType() { return Type; };
	void SetType(int type) { Type = type; };
	bool GetIsHolding()
	{
		return isHolding;
	}
	void SetIsHolding(int isHolding)
	{
		this->isHolding = isHolding;
	}
	void StartTimeRevive()
	{
		reviveStart = GetTickCount();
	}
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
};