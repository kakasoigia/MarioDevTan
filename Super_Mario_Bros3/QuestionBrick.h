#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScence.h"
#include "Leaf.h"
#include "MushRoom.h"
#include "Coin.h"

#define QUESTION_BRICK_BBOX_WIDTH  16
#define QUESTION_BRICK_BBOX_HEIGHT 16

#define QUESTION_BRICK_STATE_USED 100

#define QUESTION_BRICK_ANI_ALIVE	0
#define QUESTION_BRICK_ANI_DEAD		1
#define QUESTION_BRICK_ANI_NEW_TYPE	2

#define QUESTION_BRICK_NORMAL				666
#define QUESTION_BRICK_HAVE_LEAF			777
#define QUESTION_BRICK_JUST_HAVE_MUSHROOM	888


class CQuestionBrick : public CGameObject
{
	int type;
	bool isAlive = true;
	bool isUsed = false;

public:
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	CQuestionBrick(int ctype);
	virtual void SetState(int state,vector<LPGAMEOBJECT> *coObjects);
	bool GetIsAlive()
	{
		return isAlive;
	}
	void SetIsAlive(bool isAliveBool)
	{
		isAlive = isAliveBool;
	}
	int GetType()
	{
		return type;
	}
	void SetType(int Ctype)
	{
		type = Ctype;
	}
	bool GetIsUsed()
	{
		return isUsed;
	}
	void SetIsUsed(bool isUsedBool)
	{
		isUsed = isUsedBool;
	}

};