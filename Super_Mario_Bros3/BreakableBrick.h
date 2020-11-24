#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "PlayScence.h"
#include "Bell.h"
#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

#define BREAKABLE_BRICK_NORMAL 111
#define BREAKABLE_BRICK_BELL 222

#define BREAKABLE_STATE_SHOW 444
#define BREAKABLE_STATE_BROKEN	555
#define BREAKABLE_STATE_EMPTY_BOX	666
#define BREAKABLE_STATE_COIN	777

#define BREAKABLE_ANI_NORMAL 0
#define BREAKABLE_ANI_EMPTY_BOX 1
#define BREAKABLE_ANI_COIN 2

#define TIME_REVIVE_FROM_COIN 3000
class CBreakableBrick : public CGameObject
{
	int Type;
	int time_revive_start = 0;
public:
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	CBreakableBrick(int type);
	int GetType()
	{
		return Type;
	}
	void SetType(int Type)
	{
		this->Type = Type;
	}
	void StartReviteTime()
	{
		time_revive_start = GetTickCount();
	}
};