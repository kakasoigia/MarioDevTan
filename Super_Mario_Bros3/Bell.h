#pragma once
#include "GameObject.h"
#include "define.h"



class CBell : public CGameObject
{
	bool isUp = false;
	bool isActive = false;
public:
	CBell() { state = BELL_STATE_DISAPPEAR; };
	virtual void Render();
	virtual void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void SetIsUp(bool isUp)
	{
		this->isUp = isUp;
	}
	bool GetIsUp()
	{
		return isUp;
	}
	void SetActive(bool isActive)
	{
		this->isActive = isActive;
	}
	bool GetActive()
	{
		return isActive;
	}
};