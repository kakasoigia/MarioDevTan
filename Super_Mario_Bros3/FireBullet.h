#pragma once
#include "GameObject.h"
#include "PlayScence.h"
#include "Mario.h"

#define BULLET_BBOX_WIDTH	9
#define BULLET_BBOX_HEIGHT 9

#define BULLET_GRAVITY 0.0009f
#define BULLET_FLYING_SPEED 0.35f

#define BULLET_STATE_FLYING 100
#define BULLET_STATE_DISAPPEARING 200

#define BULLET_ANI_FLYING_RIGHT 0
#define BULLET_ANI_FLYING_LEFT 1
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