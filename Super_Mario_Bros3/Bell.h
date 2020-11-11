#pragma once
#include "GameObject.h"

#define BELL_BBOX_WIDTH  16
#define BELL_BBOX_HEIGHT 16



class CBell : public CGameObject
{
	bool isUp = false;
	bool isActive = false;
public:
	virtual void Render();
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