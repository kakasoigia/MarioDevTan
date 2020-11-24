#pragma once
#include "GameObject.h"

#define BELL_BBOX_WIDTH  16
#define BELL_BBOX_HEIGHT 16
#define BELL_BBOX_HEIGHT_PRESSED 7

#define BELL_STATE_DISAPPEAR 100
#define BELL_STATE_SHOW 200
#define BELL_STATE_PRESSED 300

#define BELL_ANI_SHOW 0
#define BELL_ANI_PRESSED 1

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