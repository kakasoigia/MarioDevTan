#include "BreakableBrick.h"
#include "Utils.h"
CBreakableBrick::CBreakableBrick(int type)
{
	Type = type;
	state = BREAKABLE_STATE_SHOW;
}
void CBreakableBrick::Render()
{
	int ani = 0;
	switch (state)
	{
		case BREAKABLE_STATE_BROKEN:
			return; 
		case BREAKABLE_STATE_COIN:
			ani = BREAKABLE_ANI_COIN; break;
		case BREAKABLE_STATE_SHOW:
			ani = BREAKABLE_ANI_NORMAL; break;
		case BREAKABLE_STATE_EMPTY_BOX:
			ani = BREAKABLE_ANI_EMPTY_BOX; break;
		
	}

	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void CBreakableBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (state == BREAKABLE_STATE_BROKEN)
	{
		l = 0;
		t = 0;
		r = 0;
		b = 0;
	}
	else
	{
		l = x;
		t = y;
		r = x + BRICK_BBOX_WIDTH;
		b = y + BRICK_BBOX_HEIGHT;
	}

}
void CBreakableBrick::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);

	////
	//// TO-DO: make sure Goomba can interact with the world and to each of them too!
	////
	if (isBouncing)
	{
	
			if (time_Y_Up < 4)
			{

				y -= 2;
				time_Y_Up++;
				
			}
			else if (time_Y_Up < 8)
			{
				y += 2;
				time_Y_Up++;
				
			}
			else
			{
				time_Y_Up = 0;
				isBouncing = false;
			}
		
		
	}
	if (GetTickCount() - time_revive_start > TIME_REVIVE_FROM_COIN)
	{
		if (state == BREAKABLE_STATE_COIN) state =BREAKABLE_STATE_SHOW;
		time_revive_start = 0;
	}

}
