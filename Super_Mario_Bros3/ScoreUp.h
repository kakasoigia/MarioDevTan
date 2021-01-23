#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScence.h"
#include "define.h"




class CScoreUp : public CGameObject
{
	bool isUsed = false;
	int value = 0;
	

	DWORD timing_start = 0;
	bool score_recog = false;
public:

	CScoreUp();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void SetState(int state);
	void StartTiming()
	{
		if (timing_start == 0)
			timing_start = GetTickCount();
	}
	void SetIsUsed(bool isUsedBool)
	{
		isUsed = isUsedBool;
	}
	bool GetIsUsed()
	{
		return isUsed;
	}
	void SetValue(int valueInt)
	{
		value = valueInt;
	}
	


};