#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScence.h"
#include "define.h"



class CFloatingWood : public CGameObject
{
	bool isAppear = false;
	DWORD upping_start = 0;

	int id=0;

public:
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	CFloatingWood();
	virtual void SetState(int state);
	int GetId()
	{
		return this->id;
	}

};
