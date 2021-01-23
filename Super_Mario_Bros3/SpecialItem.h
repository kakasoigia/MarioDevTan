#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScence.h"
#include "define.h"



class CSpecialItem : public CGameObject
{
	bool isAppear = true;
	DWORD upping_start = 0;
	DWORD switching_state_start = 0;
	bool isShowYouGotACard;
	float origin_x;
	float origin_y;
public:
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	CSpecialItem();
	virtual void SetState(int state);

	void StartUpping()
	{
		if (upping_start == 0)
			upping_start = GetTickCount();
	}
	void StartSwitchState()
	{
		if (switching_state_start == 0)
			switching_state_start = GetTickCount();
	}
	void SetOriginPos(float x,float y)
	{
		origin_x = x;
		origin_y = y;
	}

};