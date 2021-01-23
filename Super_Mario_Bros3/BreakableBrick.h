#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "PlayScence.h"
#include "Bell.h"
#include "define.h"
class CBreakableBrick : public CGameObject
{
	int Type;
	int time_revive_start = 0;
	bool isBouncing = false;
	int time_Y_Up = 0;
public:
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	CBreakableBrick(int type);
	virtual void SetState(int state);
	void CallBrickPieces();
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
	bool GetIsBouncing()
	{
		return isBouncing;
	}
	void SetIsBouncing(bool isBouncing)
	{
		this->isBouncing = isBouncing;
	}
};