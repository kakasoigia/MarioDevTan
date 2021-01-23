#pragma once
#include "GameObject.h"
#include "PlayScence.h"
#include "QuestionBrick.h"
#include "define.h"
class CCoin : public CGameObject
{
	bool disappear = false;
	bool isAppear;
	int type;
	DWORD timing_start;
public:
	CCoin(int type);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	virtual void SetState(int state);
	void StartTiming()
	{
		timing_start = GetTickCount();
	}
	void SetIsAppear(bool isAppearBool)
	{
		isAppear = isAppearBool;
	}
	bool GetIsAppear()
	{
		return isAppear;
	}
};