#pragma once
#include "GameObject.h"
#include "NoCollisionObjects.h"

#define MARIO_WALKING_SPEED		0.1f 


#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.002f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	150
#define MARIO_STATE_WALKING_LEFT	250
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_DIE				400
#define MARIO_STATE_FLY	500
//#define MARIO_STATE_FLYING_LEFT		600
#define MARIO_STATE_BRAKING_RIGHT	700	
#define MARIO_STATE_BRAKING_LEFT	800
#define MARIO_STATE_HOLDING_RIGHT	900
#define MARIO_STATE_HOLDING_LEFT	1000
#define MARIO_STATE_HOLDING_NOTHING	1100
#define MARIO_STATE_KICK			1200
#define MARIO_STATE_RUNNING_RIGHT	1300
#define MARIO_STATE_RUNNING_LEFT	1400
#define MARIO_STATE_SITDOWN			1500
#define MARIO_STATE_SPEEDING_DOWN	1600
#define MARIO_STATE_TURNING_TAIL	1700
#define MARIO_STATE_FALL_DOWN	1800
#define	MARIO_STATE_PIPE_SLIDE_DOWN 1900
#define MARIO_STATE_PIPE_SLIDE_UP	2000



#define MARIO_ANI_BIG_IDLE_RIGHT		0
#define MARIO_ANI_BIG_IDLE_LEFT			1
#define MARIO_ANI_BIG_WALKING_RIGHT		2	
#define MARIO_ANI_BIG_WALKING_LEFT		3
#define MARIO_ANI_BIG_JUMPING_RIGHT		4
#define MARIO_ANI_BIG_JUMPING_LEFT		5
#define MARIO_ANI_BIG_FLYING_RIGHT		6	
#define MARIO_ANI_BIG_FLYING_LEFT		7
#define MARIO_ANI_BIG_BRAKING_RIGHT		8	
#define MARIO_ANI_BIG_BRAKING_LEFT		9
#define MARIO_ANI_BIG_HOLDING_RIGHT		10	
#define MARIO_ANI_BIG_HOLDING_LEFT		11
#define MARIO_ANI_BIG_KICKING_RIGHT		12	
#define MARIO_ANI_BIG_KICKING_LEFT		13
#define MARIO_ANI_BIG_RUNNING_RIGHT		14	
#define MARIO_ANI_BIG_RUNNING_LEFT		15
#define MARIO_ANI_BIG_SITDOWN_RIGHT		16	
#define MARIO_ANI_BIG_SITDOWN_LEFT		17

#define MARIO_ANI_SMALL_IDLE_RIGHT			20
#define MARIO_ANI_SMALL_IDLE_LEFT			21
#define MARIO_ANI_SMALL_WALKING_RIGHT		22	
#define MARIO_ANI_SMALL_WALKING_LEFT		23
#define MARIO_ANI_SMALL_JUMPING_RIGHT		24
#define MARIO_ANI_SMALL_JUMPING_LEFT		25
#define MARIO_ANI_SMALL_FLYING_RIGHT		26	
#define MARIO_ANI_SMALL_FLYING_LEFT			27
#define MARIO_ANI_SMALL_BRAKING_RIGHT		28	
#define MARIO_ANI_SMALL_BRAKING_LEFT		29
#define MARIO_ANI_SMALL_HOLDING_RIGHT		30	
#define MARIO_ANI_SMALL_HOLDING_LEFT		31
#define MARIO_ANI_SMALL_KICKING_RIGHT		32	
#define MARIO_ANI_SMALL_KICKING_LEFT		33
#define MARIO_ANI_SMALL_RUNNING_RIGHT		34	
#define MARIO_ANI_SMALL_RUNNING_LEFT		35



#define MARIO_ANI_TAIL_IDLE_RIGHT		40
#define MARIO_ANI_TAIL_IDLE_LEFT		41
#define MARIO_ANI_TAIL_WALKING_RIGHT	42	
#define MARIO_ANI_TAIL_WALKING_LEFT		43
#define MARIO_ANI_TAIL_JUMPING_RIGHT	44
#define MARIO_ANI_TAIL_JUMPING_LEFT		45
#define MARIO_ANI_TAIL_FLYING_RIGHT		46	
#define MARIO_ANI_TAIL_FLYING_LEFT		47
#define MARIO_ANI_TAIL_BRAKING_RIGHT	48	
#define MARIO_ANI_TAIL_BRAKING_LEFT		49
#define MARIO_ANI_TAIL_HOLDING_RIGHT	50	
#define MARIO_ANI_TAIL_HOLDING_LEFT		51
#define MARIO_ANI_TAIL_KICKING_RIGHT	52	
#define MARIO_ANI_TAIL_KICKING_LEFT		53
#define MARIO_ANI_TAIL_RUNNING_RIGHT	54	
#define MARIO_ANI_TAIL_RUNNING_LEFT		55
#define MARIO_ANI_TAIL_SITDOWN_RIGHT	56
#define MARIO_ANI_TAIL_SITDOWN_LEFT		57
#define MARIO_ANI_TAIL_TURNING_RIGHT	58	
#define MARIO_ANI_TAIL_TURNING_LEFT		59


#define MARIO_ANI_FIRE_IDLE_RIGHT		60
#define MARIO_ANI_FIRE_IDLE_LEFT		61
#define MARIO_ANI_FIRE_WALKING_RIGHT	62	
#define MARIO_ANI_FIRE_WALKING_LEFT		63
#define MARIO_ANI_FIRE_JUMPING_RIGHT	64
#define MARIO_ANI_FIRE_JUMPING_LEFT		65
#define MARIO_ANI_FIRE_FLYING_RIGHT		66	
#define MARIO_ANI_FIRE_FLYING_LEFT		67
#define MARIO_ANI_FIRE_BRAKING_RIGHT	68	
#define MARIO_ANI_FIRE_BRAKING_LEFT		69
#define MARIO_ANI_FIRE_HOLDING_RIGHT	70
#define MARIO_ANI_FIRE_HOLDING_LEFT		71
#define MARIO_ANI_FIRE_KICKING_RIGHT	72	
#define MARIO_ANI_FIRE_KICKING_LEFT		73
#define MARIO_ANI_FIRE_RUNNING_RIGHT	74	
#define MARIO_ANI_FIRE_RUNNING_LEFT		75
#define MARIO_ANI_FIRE_SITDOWN_RIGHT	76
#define MARIO_ANI_FIRE_SITDOWN_LEFT		77
#define MARIO_ANI_FIRE_FIRING_BULLET_LEFT	79
#define MARIO_ANI_FIRE_FIRING_BULLET_RIGHT	78

#define MARIO_ANI_BIG_MAX_SPEED_RIGHT	80
#define MARIO_ANI_BIG_MAX_SPEED_LEFT	81
#define MARIO_ANI_FIRE_MAX_SPEED_RIGHT	82
#define MARIO_ANI_FIRE_MAX_SPEED_LEFT	83
#define MARIO_ANI_SMALL_MAX_SPEED_RIGHT	84
#define MARIO_ANI_SMALL_MAX_SPEED_LEFT	85
#define MARIO_ANI_TAIL_MAX_SPEED_RIGHT	86
#define MARIO_ANI_TAIL_MAX_SPEED_LEFT	87
#define MARIO_ANI_FALLING_LEFT			88
#define MARIO_ANI_FALLING_RIGHT			89

#define MARIO_ANI_DIE					90
#define MARIO_ANI_SMALL_PIPE		91
#define MARIO_ANI_BIG_PIPE			92
#define MARIO_ANI_TAIL_PIPE						93
#define MARIO_ANI_FIRE_PIPE						94

#define MARIO_TRANSFORM_LEFT					95
#define MARIO_TRANSFORM_RIGHT					96

#define MARIO_SMOKE_TRANSFORM_LEFT				97
#define MARIO_SMOKE_TRANSFORM_RIGHT				98

#define MARIO_ANI_BIG_JUMP_MAX_POWER_RIGHT			99
#define MARIO_ANI_BIG_JUMP_MAX_POWER_LEFT			100
#define MARIO_ANI_SMALL_JUMP_MAX_POWER_RIGHT		101
#define MARIO_ANI_SMALL_JUMP_MAX_POWER_LEFT		    102
#define MARIO_ANI_TAIL_JUMP_MAX_POWER_RIGHT			103
#define MARIO_ANI_TAIL_JUMP_MAX_POWER_LEFT			104
#define MARIO_ANI_FIRE_JUMP_MAX_POWER_RIGHT			105
#define MARIO_ANI_FIRE_JUMP_MAX_POWER_LEFT			106

#define	MARIO_LEVEL_BIG		2
#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_TAIL	3
#define	MARIO_LEVEL_FIRE	4

#define MARIO_BIG_BBOX_WIDTH  15
#define MARIO_BIG_BBOX_HEIGHT 27

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15
#define MARIO_TURNING_BONUS_HEIGHT 8
#define MARIO_UNTOUCHABLE_TIME 3000
#define MARIO_KICK_TIME 300
#define MARIO_TURNING_TIME		 500
#define MARIO_FLYING_TIME		 3500
#define MARIO_TIME_BACK_TO_WORLDMAP	2000
#define MARIO_TIME_SLIDE_UP_DOWN 3000
#define MARIO_TIME_TRANSFORM 1000
#define MARIO_TIME_FIRING 200
#define	MARIO_DIFFERENCE_HEIGHT	12

#define MARIO_BIG_BBOX_WIDTH  15
#define MARIO_BIG_BBOX_HEIGHT 27
#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15
#define MARIO_TAIL_BBOX_WIDTH  21
#define MARIO_TAIL_BBOX_HEIGHT 27
#define MARIO_FIRE_BBOX_WIDTH  14
#define MARIO_FIRE_BBOX_HEIGHT 27

#define SPEECH_ADDTION_PER_LEVEL	0.002f
#define MARIO_MAX_SPEED	0.3f
#define MARIO_INERTIA 0.03f
#define MARIO_FLYING_SPEED 0.075f
#define MARIO_TAIL_FALL_SPEED 0.05f

#define MARIO_SPRITE_SCORE_UP_100	60150
#define MARIO_SPRITE_SCORE_UP_200	60151
#define MARIO_SPRITE_SCORE_UP_400	60152
#define MARIO_SPRITE_SCORE_UP_800	60153
#define MARIO_SPRITE_SCORE_UP_1000	60154
#define MARIO_SPRITE_SCORE_UP_2000	60155
#define MARIO_SPRITE_SCORE_UP_4000	60156
#define MARIO_SPRITE_SCORE_UP_8000	60157
#define MARIO_SPRITE_SCORE_UP_LV_UP	60158

#define POS_X_TUNNEL_MAP_1 1330
#define POS_Y_TUNNEL_MAP_1 1050
#define POS_X_OUT_OF_TUNNEL_MAP_1 2330
#define POS_Y_OUT_OF_TUNNEL_MAP_1 122
#define POS_X_SCENE_END_MAP_4 2200
#define POS_Y_SCENE_END_MAP_4 300
#define POS_X_EDGE_MAP_4 2055
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