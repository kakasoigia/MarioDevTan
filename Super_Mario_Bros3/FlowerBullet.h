#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "PlayScence.h"
#include "Scence.h"
#include "Flower.h"
#include <algorithm>
#include "define.h"

class CFlowerBullet : public CGameObject
{
	bool isUsed;

public:
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void SetState(int state);

	CFlowerBullet()
	{
		SetState(FLOWER_BULLET_STATE_HIDDEN);
	}
	bool GetIsUsed()
	{
		return isUsed;
	}
	void SetIsUsed(bool isUsed)
	{
		this->isUsed = isUsed;
	}
};