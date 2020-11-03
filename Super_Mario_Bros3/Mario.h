#pragma once
#include "GameObject.h"

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
#define MARIO_STATE_FLYING_RIGHT	500
#define MARIO_STATE_FLYING_LEFT		600
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

#define MARIO_ANI_DIE					78


#define	MARIO_LEVEL_BIG		2
#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_TAIL	3
#define	MARIO_LEVEL_FIRE	4

#define MARIO_BIG_BBOX_WIDTH  15
#define MARIO_BIG_BBOX_HEIGHT 27

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15

#define MARIO_UNTOUCHABLE_TIME 5000
#define MARIO_KICK_TIME 300
#define MARIO_TURNING_TIME		 300
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
#define MARIO_MAX_SPEED	0.2f
#define MARIO_INERTIA 0.03f

class CMario : public CGameObject
{
	int level;
	int untouchable;
	int kicking_start;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y;
protected:
	bool isJumping = false;
	bool isBraking = false;
	bool isKicking = false;
	int isHolding = -1;
	bool isTurning = false;
	bool isFiring = false;
	int current_level_speed_up;
	DWORD turning_start = 0;
public:
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l);
	int GetLevel() { return level; };
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void StartKicking() { kicking_start = GetTickCount(); }
	void StartTurning() { turning_start = GetTickCount(); }
	bool GetJumpingState() { return isJumping; };
	void SetJumpingState(bool state) { isJumping = state; };
	void Reset();
	void SpeedDown();
	void Fire();
	bool SpeedInertia();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	 int GetIsHolding()
	 {
		 return isHolding;
	 }
	 void SetIsHolding(int isHolding)
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
};