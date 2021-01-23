#pragma once

#include "GameObject.h"
#include "algorithm"
#include "Node.h"
#include "define.h"



class CWorldMapObjects : public CGameObject
{
	int type;

	DWORD gold_digger_walk_start = 0;
	DWORD help_appear_start = 0;
	int isHelpAppear = 1;

	DWORD mario_move_start = 0;


	bool mario_move_control = true;
public:
	CWorldMapObjects(int ctype);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) {}
	virtual void SetState(int state);
	int GetType()
	{
		return type;
	}
	void SetType(int typeInt)
	{
		type = typeInt;
	}
	void GoldDiggerStartWalking()
	{
		if (gold_digger_walk_start == 0)
			gold_digger_walk_start = GetTickCount();
	}
	void HelpAppearStart()
	{
		if (help_appear_start == 0)
			help_appear_start = GetTickCount();
	}
	void MarioMoveStart()
	{
		if (mario_move_start == 0)
			mario_move_start = GetTickCount();
	}
	bool GetMarioMoveControl()
	{
		return mario_move_control;
	}


};
