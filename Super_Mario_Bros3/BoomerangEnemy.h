#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScence.h"


#define  BOOMERANG_ENEMY_BBOX_WIDTH		16
#define  BOOMERANG_ENEMY_BBOX_HEIGHT	24

#define  BOOMERANG_ENEMY_STATE_IDLE				0
#define  BOOMERANG_ENEMY_STATE_MOVE_FORWARD		100
#define  BOOMERANG_ENEMY_STATE_MOVE_BACKWARD    200
#define	 BOOMERANG_ENEMY_STATE_DIE				300

#define BOOMERANG_ENEMY_GRAVITY		0.002f


#define BOOMERANG_ENEMY_ANI_NORMAL_RIGHT				0
#define BOOMERANG_ENEMY_ANI_THROW_BOOMERANG_RIGHT		1
#define BOOMERANG_ENEMY_ANI_DIE_RIGHT					2
#define BOOMERANG_ENEMY_ANI_DIE_LEFT					3
#define BOOMERANG_ENEMY_ANI_NORMAL_LEFT					4	
#define BOOMERANG_ENEMY_ANI_THROW_BOOMERANG_LEFT		5

class CBoomerangEnemy : public CGameObject
{
	bool isAllowToHaveBBox = true;

	DWORD time_switch_state = 0;

	bool isAlive = true;

	bool isAniThrow = false;

	DWORD time_render_ani_thow = 0;

	DWORD timing_score;

	DWORD pre_get_tick_count = 0;
	DWORD time_conpensate = 0;
public:
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	CBoomerangEnemy();
	virtual void SetState(int state);

	void StartTimeSwitchingState()
	{
		if (time_switch_state == 0)
		{
			time_switch_state = GetTickCount();
		}
	}
	void StartTimeRenderingThrowAni()
	{
		if (time_render_ani_thow == 0)
		{
			time_render_ani_thow = GetTickCount();
		}
	}
	bool GetIsAlive()
	{
		return isAlive;
	}
	void SetIsAlive(bool isAliveBool)
	{
		isAlive = isAliveBool;
	}
	
	void StartTimingScore()
	{
		timing_score = GetTickCount();
	}
	bool GetIsAllowToHaveBBox()
	{
		return isAllowToHaveBBox;
	}
	void SetIsAllowToHaveBBox(bool isAllowToHaveBBoxBool)
	{
		isAllowToHaveBBox = isAllowToHaveBBoxBool;
	}
};