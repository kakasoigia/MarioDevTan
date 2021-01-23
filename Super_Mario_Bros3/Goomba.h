#pragma once
#include "GameObject.h"
#include "Rectangle.h"
#include "Pipe.h"
#include "Brick.h"
#include	"define.h"
class CGoomba : public CGameObject
{

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	int dying;
	DWORD dying_start = 0;
	int Type;
	int jumping_start = 0;
public:
	CGoomba(int type);
	virtual void SetState(int state);
	void StartDyingTime() { dying_start = GetTickCount(); }
	int GetType() { return Type; };
	void SetType(int type) { this->Type= type; };
};