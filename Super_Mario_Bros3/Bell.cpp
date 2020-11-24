#include "Bell.h"
#include "Coin.h"
#include "BreakableBrick.h"
#include "Utils.h"
void CBell::Render()
{
	int ani = -1;
	if (state ==BELL_STATE_DISAPPEAR) return;
	else if (state == BELL_STATE_SHOW)
	{
		ani = 0;
	}
	else
	{
		ani = 1;
	}
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}
void CBell::GetBoundingBox(float &l, float &t, float &r, float &b)
{

	if (state == BELL_STATE_SHOW)
	{
		
		l = x;
		t = y;
		r = x + BELL_BBOX_WIDTH;
		b = y + BELL_BBOX_HEIGHT;
	}
	else if (state == BELL_STATE_DISAPPEAR)
	{
		l = t = r = b = 0;
	}
	else
	{
		l = x;
		t = y;
		r = x + BELL_BBOX_WIDTH;
		b = y + BELL_BBOX_HEIGHT;
	}
	
}
void CBell::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	
	CGameObject::Update(dt);
	if (isActive)
	{
		
		for (UINT i = 0; i < coObjects->size(); i++)
		{
		
			LPGAMEOBJECT obj = coObjects->at(i);
				if (dynamic_cast<CBreakableBrick*>(obj))
				{
					CBreakableBrick* breakableBrick = dynamic_cast<CBreakableBrick*>(obj);
					if (breakableBrick->GetState() == BREAKABLE_STATE_SHOW)
					{
						breakableBrick->SetState(BREAKABLE_STATE_COIN);
						breakableBrick->StartReviteTime();
					}
				}
				
		}
		isActive = false;
		state = BELL_STATE_PRESSED;
		y += 9; 
	}
}
void CBell::SetState(int state)
{
	this->state = state;
	
}