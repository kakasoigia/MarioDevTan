#pragma once

#include "GameObject.h"

#define KOOPAS_WALKING_SPEED 0.03f;

#define KOOPAS_TYPE_GREEN_WALK 1;
#define KOOPAS_TYPE_GREEN_FLY 2;
#define KOOPAS_TYPE_RED_WALK 3;
#define KOOPAS_TYPE_RED_FLY 4;

#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT 26
#define KOOPAS_BBOX_HEIGHT_DIE 16

#define KOOPAS_STATE_WALKING 100
#define KOOPAS_STATE_DIE 200
#define	KOOPAS_STATE_DISAPPEAR	300


#define KOOPAS_ANI_GREEN_WALK_WALKING_LEFT 0
#define KOOPAS_ANI_GREEN_WALK_WALKING_RIGHT 1
#define	KOOPAS_ANI_GREEN_FLY_WALKING_LEFT 2
#define	KOOPAS_ANI_GREEN_SHELL_PRONE 3
#define	KOOPAS_ANI_GREEN_SHELL_UPWARD 4
#define KOOPAS_ANI_RED_WALK_WALKING_LEFT 5
#define KOOPAS_ANI_RED_FLY_WALKING_LEFT 6
#define KOOPAS_ANI_RED_FLY_SHELL_UP 7

#define	KOOPAS_GRAVITY	0.002f


class CKoopas : public CGameObject
{

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	int Type;
public:
	CKoopas(int type);

	virtual void SetState(int state);

};