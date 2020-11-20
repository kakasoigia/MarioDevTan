#include "Coin.h"
#include "Mario.h"
void CCoin::Render()
{
	if (!isAppear)
		return;
	animation_set->at(0)->Render(x, y);

	//RenderBoundingBox();
}
CCoin::CCoin(int type)
{
	this->type = type;
	if (type == COIN_NORMAL)
	{
		isAppear = true;
	}
	else
		isAppear = false;
	SetState(COIN_STATE_IDLE);
}
void CCoin::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (isAppear == false)
	{
		l = t = r = b = 0;
	}
	else
	{
		l = x;
		t = y;
		r = x + COIN_BBOX_WIDTH;
		b = y + COIN_BBOX_HEIGHT;
	}
	
}
void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);
	x += dx;
	y += dy;
	if (!isAppear) return;

	if (state == COIN_STATE_UP)
	{
		if (GetTickCount() - timing_start >= TIME_COIN_ON_AIR)
		{
			SetState(COIN_STATE_DOWN);
		}
	}
	else if (state == COIN_STATE_DOWN)
	{
		if (GetTickCount() - timing_start >= TIME_COIN_ON_AIR-100)
		{
			isAppear = false;
		}
	}


}
void CCoin::SetState(int state)
{
	this->state = state;
	if (state == COIN_STATE_IDLE)
	{
		vx = vy = 0;
	}
	else if (state == COIN_STATE_UP)
	{
		vy = -0.2f;
		StartTiming();
		isAppear = true;
	}
	else
	{
		StartTiming();
		vy = 0.2f;
	}
}
