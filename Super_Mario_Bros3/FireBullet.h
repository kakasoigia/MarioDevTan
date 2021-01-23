#pragma once
#include "GameObject.h"
#include "PlayScence.h"
#include "Mario.h"
#include "BreakableBrick.h"

class CFireBullet : public CGameObject
{
	bool isUsed = false;
public:
	CFireBullet();
	void SetState(int state);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	bool GetIsUsed()
	{
		return isUsed;
	}
	void SetIsUsed(bool isUsed)
	{
		this -> isUsed = isUsed;
	}
};