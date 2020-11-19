#include "Coin.h"
#include "Mario.h"
void CCoin::Render()
{
	if (!isAppear)
		return;
	animation_set->at(0)->Render(x, y);

	//RenderBoundingBox();
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


	if (state == COIN_STATE_UP)
	{
		if (GetTickCount() - timing_start >= 300)
		{
			SetState(COIN_STATE_DOWN);
			StartTiming();
		}
	}

	if (state == COIN_STATE_DOWN)
	{
		if (GetTickCount() - timing_start >= 300)
		{
			isAppear = false;
		}

	}


}
void CCoin::SetState(int state)
{
	if (state == COIN_STATE_UP)
	{
		StartTiming();
		isAppear = true;
	}
}
