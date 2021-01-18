#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScence.h"


#define FLOATING_WOOD_BBOX_WIDTH		48
#define  FLOATING_WOOD_BBOX_HEIGHT		16



#define   FLOATING_WOOD_STATE_NORMAL		0
#define   FLOATING_WOOD_STATE_DOWN			100

#define	  FLOATING_WOOD_STATE_NORMAL_SPEED   -0.05f
#define   FLOATING_WOOD_STATE_DOWN_SPEED	   0.07f



class CFloatingWood : public CGameObject
{
	bool isAppear = false;
	DWORD upping_start = 0;

	int id;

public:
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	CFloatingWood(int moving_horizontal_rectangle_id);
	virtual void SetState(int state);
	int GetId()
	{
		return this->id;
	}

};
