#pragma once
#include "GameObject.h"
#include "NoCollisionObjects.h"
#include "define.h"

class CMario : public CGameObject
{
	int level;
	int untouchable;
	int kicking_start;
	int flying_start;
	DWORD untouchable_start;
	int CoinCounter = 0;
	long Score = 0;
	int life_counter = 4;
	int time_cam_count = 0;
	int Mario_Level = 0;
	vector <int> itemList;
	float start_x;			// initial position of Mario at scene
	float start_y;
	float camX_update = 0;
protected:
	bool isJumping = false;
	bool isBraking = false;
	bool isKicking = false;
	bool isHolding = false;
	bool isTurning = false;
	bool isFiring = false;
	bool isFlying = false;
	bool isHitted = false;
	bool canFly = false;
	bool isLanding = false;
	bool isHoldAni = false;
	bool isAutoWalk = false;
	bool isTransforming = false;
	bool transformUpLevel = false;
	bool isCamPushed = false;
	bool isJumpingMaxStack = false;
	DWORD start_time_die_back_to_worldmap = 0;
	int current_level_speed_up;
	DWORD turning_start = 0;
	DWORD transforming_start = 0;
	DWORD firing_start = 0;

	bool canPipeSlideDown = false;
	bool canPipeSlideUp = false;

	bool isAtTunnel = false;
	bool setPositionOutOfTunnel = false;
	DWORD pipe_slide_down_start = 0;
	DWORD pipe_slide_up_start = 0;
public:
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	virtual void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents,
		vector<LPCOLLISIONEVENT> &coEventsResult,
		float &min_tx, float &min_ty,
		float &nx, float &ny, float &rdx, float &rdy);

	void SetState(int state);
	void SetLevel(int l);
	int GetLevel() { return level; };
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void StartKicking() { kicking_start = GetTickCount(); }
	void StartFlying();
	void StartTurning() { turning_start = GetTickCount(); }
	bool GetJumpingState() { return isJumping; };
	void SetJumpingState(bool state) { isJumping = state; };
	void Reset(); void SetOnSpecialPosition(int place);
	void SpeedDown();
	void Fire();
	bool SpeedInertia();
	void SetTransformingDown();
	void SetTransformingUp(int type);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void CallHitEffect(int type,int nx, float pos_x, float pos_y);
	int GetIsHolding()
	{
		return isHolding;
	}
	void SetIsHolding(bool isHolding)
	{
		this->isHolding = isHolding;
	}
	void IncreaseCurrentLevelSpeed()
	{
		current_level_speed_up++;
	}
	void DecreaseCurrentLevelSpeed()
	{
		current_level_speed_up--;
	}
	int GetLevelSpeedUp()
	{
		return current_level_speed_up;
	}
	bool GetIsKicking()
	{
		return isKicking;
	}
	void SetIsKicking(bool isKickingBool)
	{
		this->isKicking = isKickingBool;
	}
	bool GetIsFiring()
	{
		return isFiring;
	}
	void SetIsFiring(bool isFiring)
	{
		this->isFiring = isFiring;
	}
	void SetIsTurning(bool isTurningBool)
	{
		this->isTurning = isTurningBool;
	}
	bool GetIsTurning()
	{
		return  isTurning;
	}
	void SetCanFly(bool canFly)
	{
		this->canFly = canFly;
	}
	bool GetCanFly()
	{
		return  canFly;
	}
	void SetIsLanding(bool isLanding)
	{
		this->isLanding = isLanding;
	}
	bool GetIsLanding()
	{
		return  isLanding;
	}
	void SetIsJumping(bool isJumping)
	{
		this->isJumping = isJumping;
	}
	bool GetIsJumping()
	{
		return  isJumping;
	}
	bool GetIsHitted()
	{
		return isHitted;
	}
	void SetIsHitted(bool isHittedBool)
	{
		isHitted = isHittedBool;
	}
	bool GetIsHoldAni()
	{
		return isHoldAni;
	}
	void SetIsHoldAni(bool isHoldAni)
	{
		this->isHoldAni = isHoldAni;
	}
	bool GetAutoWalk()
	{
		return isAutoWalk;
	}
	void SetAutoWalk(bool isAutoWalk)
	{
		this->isAutoWalk = isAutoWalk;
	}
	//pipe tunnel
	void StartPipeSlideDown()
	{
		if (pipe_slide_down_start == 0)
			pipe_slide_down_start = GetTickCount();
	}
	void StartPipeSlideUp()
	{
		if (pipe_slide_up_start == 0)
			pipe_slide_up_start = GetTickCount();
	}

	bool GetIsAtTunnel()
	{
		return isAtTunnel;
	}
	void SetIsAtTunnel(bool isAtTheTunnelBool)
	{
		this->isAtTunnel = isAtTheTunnelBool;
	}
	bool GetCanPipeSlideDown()
	{
		return canPipeSlideDown;
	}
	void SetCanPipeSlideDown(bool canPipeSlideDown)
	{
		this->canPipeSlideDown = canPipeSlideDown;
	}
	bool GetCanPipeSlideUp()
	{
		return canPipeSlideUp;
	}
	void SetCanPipeSlideUp(bool canPipeSlideUp)
	{
		this->canPipeSlideUp = canPipeSlideUp;
	}
	bool GetIsTransforming()
	{
		return isTransforming;
	}
	void SetIsTransforming(bool isTransformingBool)
	{
		isTransforming = isTransformingBool;
	}
	bool GetIsTransFormUpLevel()
	{
		return transformUpLevel;
	}
	void SetIsTransFormUpLevel(bool transformRecogBool)
	{
		transformUpLevel = transformRecogBool;
	}
	void StartTransforming()
	{
		if (transforming_start == 0)
			transforming_start = GetTickCount();
	}
	bool GetIsCamPushed()
	{
		return isCamPushed;
	}
	void SetIsCamPushed(bool isCamPushed)
	{
		this->isCamPushed = isCamPushed;
	}
	// HUD 
	int GetCoinCounter() { return CoinCounter; };
	void CoinCounterUp() { CoinCounter++; Score += 50; };
	long GetScore() { return Score; };
	void IncScore(int score, float pos_x, float pos_y);
	void AddLifeCounter() { life_counter++; };
	int GetLifeCounter() { return life_counter; };
	void AddItem(int item_type) { if (itemList.size() <= 3) itemList.push_back(item_type); };
	vector<int> GetItemList() { return itemList; };
	void StartTimeBackToWorld() { start_time_die_back_to_worldmap = GetTickCount(); };
	DWORD GetTimeBackToWorld() { return start_time_die_back_to_worldmap; };

};