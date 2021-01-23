#pragma once

#include "GameObject.h"
#include "Goomba.h"
#include "Mario.h"
#include "PlayScence.h"
#include "define.h"

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
	bool isDown = true;
	bool isRedFlyAtBirth = false;
	DWORD switching_state_time = 0;

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
	bool GetIsDown()
	{
		return isDown;
	}
	void SetIsDown(bool isDownBool)
	{
		isDown = isDownBool;
	}
	void StartSwitchingState()
	{
		if (switching_state_time == 0)
		{
			switching_state_time = GetTickCount();
		}
	}
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
};