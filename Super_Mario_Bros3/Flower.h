#pragma once
#include "GameObject.h"
#include "algorithm"
#include "FlowerBullet.h"

#define FLOWER_RED_BBOX_WIDTH  16
#define FLOWER_RED_BBOX_HEIGHT 32

#define FLOWER_GREEN_BBOX_WIDTH  16
#define FLOWER_GREEN_BBOX_HEIGHT 24

#define FLOWER_GREEN_CAN_SHOOT_BBOX_WIDTH  16
#define FLOWER_GREEN_CAN_SHOOT_BBOX_HEIGHT 24

#define FLOWER_RED_ANI_LEFT			0
#define FLOWER_RED_ANI_RIGHT		1
#define FLOWER_RED_ANI_LEFT_IDLE	2
#define FLOWER_RED_ANI_RIGHT_IDLE	3
#define FLOWER_GREEN_ANI			4
#define FLOWER_GREEN_CAN_SHOOT_ANI_LEFT			5
#define FLOWER_GREEN_CAN_SHOOT_ANI_RIGHT		6
#define FLOWER_GREEN_CAN_SHOOT_ANI_LEFT_IDLE	7
#define FLOWER_GREEN_CAN_SHOOT_ANI_RIGHT_IDLE	8
#define FLOWER_RED_ANI_LEFT_UP						9
#define FLOWER_RED_ANI_RIGHT_UP						10
#define FLOWER_RED_ANI_LEFT_IDLE_UP					11
#define FLOWER_RED_ANI_RIGHT_IDLE_UP				12
#define FLOWER_GREEN_CAN_SHOOT_ANI_LEFT_UP			13
#define FLOWER_GREEN_CAN_SHOOT_ANI_RIGHT_UP			14
#define FLOWER_GREEN_CAN_SHOOT_ANI_LEFT_IDLE_UP		15
#define FLOWER_GREEN_CAN_SHOOT_ANI_RIGHT_IDLE_UP	16

#define FLOWER_RED				100
#define FLOWER_GREEN			200
#define FLOWER_GREEN_CAN_SHOOT  300 


#define FLOWER_RED_TOP_LIMIT			72
#define FLOWER_RED_BOT_LIMIT			117
#define RED_TIME_SHOWING_LIMIT			3000

#define FLOWER_GREEN_TOP_LIMIT			96
#define FLOWER_GREEN_BOT_LIMIT			126
#define GREEN_TIME_SHOWING_LIMIT		2000


#define FLOWER_GREEN_CAN_SHOOT_TOP_LIMIT		79
#define FLOWER_GREEN_CAN_SHOOT_BOT_LIMIT		126
#define GREEN_CAN_SHOOT_TIME_SHOWING_LIMIT		3000

class CFlower : public CGameObject
{
	int type;
	int isShootingUp = 1;
	DWORD time_showing = 0;
	bool isUp = true;
	bool isFiring = false;
	bool isFired = false;


public:
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	CFlower(int ctype);
	virtual void SetState(int state);
	void StartShowing()
	{
		time_showing = GetTickCount();
	}
	int GetIsShootingUp()
	{
		return isShootingUp;
	}
	void SetIsShootingUp(int isShootingUpInt)
	{
		isShootingUp = isShootingUpInt;
	}
	bool GetIsFiring()
	{
		return isFiring;
	}
	void SetIsFiring(bool isFiringBool)
	{
		isFiring = isFiringBool;
	}
	int GetType()
	{
		return type;
	}
	void SetType(int typeInt)
	{
		type = typeInt;
	}
	void Shoot(vector<LPGAMEOBJECT> *coObjects);
};
