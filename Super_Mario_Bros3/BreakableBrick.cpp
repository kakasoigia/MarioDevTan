#include "BreakableBrick.h"
#include "Utils.h"
#include "BreakableBrickAnimation.h"
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
		case BREAKABLE_STATE_DISAPPEAR:
			return;
		case BREAKABLE_STATE_COIN:
			ani = BREAKABLE_ANI_COIN; break;
		case BREAKABLE_STATE_SHOW:
			ani = BREAKABLE_ANI_NORMAL; break;
		case BREAKABLE_STATE_EMPTY_BOX:
			ani = BREAKABLE_ANI_EMPTY_BOX; break;
		
	}

	animation_set->at(ani)->Render(x, y);
	DebugOut(L" render được cục gạch nha \n");
	//RenderBoundingBox();
}

void CBreakableBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (state == BREAKABLE_STATE_BROKEN || state == BREAKABLE_STATE_DISAPPEAR)
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
void CBreakableBrick::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case	BREAKABLE_STATE_BROKEN:
		CallBrickPieces();
		break;
	}
}
void CBreakableBrick::CallBrickPieces()
{
	vector<LPGAMEOBJECT> objects = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->Get_objects();
	unsigned int i = 0;
	for ( i; i < objects.size(); i++) // find breakable brick free
	{
		if (dynamic_cast<CBreakableBrickAnimation *>(objects[i]))
		{
			CBreakableBrickAnimation *brick_piece = dynamic_cast<CBreakableBrickAnimation *>(objects[i]);
			if (!brick_piece->GetIsUsed() && brick_piece->GetType() == BREAKABLE_BRICK_ANIMATION_TYPE_LEFT_TOP)
			{
				brick_piece->SetIsUsed(true);
				brick_piece->StartTiming();
				brick_piece->SetState(BREAKABLE_BRICK_ANIMATION_STATE_MOVE);
				brick_piece->SetPosition(x+ BRICK_BBOX_WIDTH/2,y+ BRICK_BBOX_HEIGHT/2);
				break;
			}
		}
		;
	}
	for (i; i < objects.size(); i++) // find breakable brick free
	{
		if (dynamic_cast<CBreakableBrickAnimation *>(objects[i]))
		{
			CBreakableBrickAnimation *brick_piece = dynamic_cast<CBreakableBrickAnimation *>(objects[i]);
			if (!brick_piece->GetIsUsed() && brick_piece->GetType() == BREAKABLE_BRICK_ANIMATION_TYPE_RIGHT_TOP)
			{
				brick_piece->SetIsUsed(true);
				brick_piece->StartTiming();
				brick_piece->SetState(BREAKABLE_BRICK_ANIMATION_STATE_MOVE);
				brick_piece->SetPosition(x + BRICK_BBOX_WIDTH / 2, y + BRICK_BBOX_HEIGHT / 2);
				break;
			}
		}
		;
	}
	for (i; i < objects.size(); i++) // find breakable brick free
	{
		if (dynamic_cast<CBreakableBrickAnimation *>(objects[i]))
		{
			CBreakableBrickAnimation *brick_piece = dynamic_cast<CBreakableBrickAnimation *>(objects[i]);
			if (!brick_piece->GetIsUsed() && brick_piece->GetType() == BREAKABLE_BRICK_ANIMATION_TYPE_RIGHT_BOTTOM)
			{
				brick_piece->SetIsUsed(true);
				brick_piece->StartTiming();
				brick_piece->SetState(BREAKABLE_BRICK_ANIMATION_STATE_MOVE);
				brick_piece->SetPosition(x + BRICK_BBOX_WIDTH / 2, y + BRICK_BBOX_HEIGHT / 2);
				break;
			}
		}
		;
	}
	for (i; i < objects.size(); i++) // find breakable brick free
	{
		if (dynamic_cast<CBreakableBrickAnimation *>(objects[i]))
		{
			CBreakableBrickAnimation *brick_piece = dynamic_cast<CBreakableBrickAnimation *>(objects[i]);
			if (!brick_piece->GetIsUsed() && brick_piece->GetType() == BREAKABLE_BRICK_ANIMATION_TYPE_LEFT_BOTTOM)
			{
				brick_piece->SetIsUsed(true);
				brick_piece->StartTiming();
				brick_piece->SetState(BREAKABLE_BRICK_ANIMATION_STATE_MOVE);
				brick_piece->SetPosition(x + BRICK_BBOX_WIDTH / 2, y + BRICK_BBOX_HEIGHT / 2);
				break;
			}
		}
		;
	}
}

