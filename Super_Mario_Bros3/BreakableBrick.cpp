#include "BreakableBrick.h"

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

	//vector<LPCOLLISIONEVENT> coEvents;
	//vector<LPCOLLISIONEVENT> coEventsResult;

	//coEvents.clear();
	//CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	//CalcPotentialCollisions(coObjects, coEvents);

	//// No collision occured, proceed normally
	//if (coEvents.size() == 0)
	//{
	//	y += dy;
	//}
	//else
	//{

	//	float min_tx, min_ty, nx = 0, ny;
	//	float rdx = 0;
	//	float rdy = 0;

	//	// TODO: This is a very ugly designed function!!!!
	//	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

	//	// block 
	//	//x += min_tx * dx + nx * 0.5f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
	//	//y += min_ty * dy + ny * 0.5f;

	//	if (nx != 0) vx = 0;
	//	if (ny != 0) vy = 0;

	//	// Collision logic with the others Goombas
	//	for (UINT i = 0; i < coEventsResult.size(); i++)
	//	{
	//		LPCOLLISIONEVENT e = coEventsResult[i];
	//		if (e->ny < 0)
	//		{
	//			if (dynamic_cast<CMario *>(e->obj))
	//			{
	//				CMario *mario = dynamic_cast<CMario *>(e->obj);
	//				if (Type == BREAKABLE_BRICK_NORMAL)
	//					state = BREAKABLE_STATE_BROKEN;
	//				else
	//				{
	//					state = BREAKABLE_STATE_EMPTY_BOX; 
	//					 //find Bell available at same place
	//					for (UINT i = 0; i < coObjects->size(); i++)
	//					{
	//						LPGAMEOBJECT obj = coObjects->at(i);
	//						if (dynamic_cast<CBell *>(obj))
	//						{
	//							////check position
	//							if (this->x == obj->x && this->y == obj->y)
	//							{
	//								CBell *bell = dynamic_cast<CBell *>(obj);
	//								bell->SetIsUp(true);
	//								bell->y = this->y - 16;
	//							}
	//								
	//						}
	//					}


	//				}
	//					

	//			}
	//		}
	//		if (e->nx != 0)
	//		{
	//			if (dynamic_cast<CMario *>(e->obj))
	//			{


	//				CMario *mario = dynamic_cast<CMario *>(e->obj);
	//				if (mario->GetIsTurning())
	//					if (Type == BREAKABLE_BRICK_NORMAL)
	//						state = BREAKABLE_STATE_BROKEN;
	//					else
	//						state = BREAKABLE_STATE_EMPTY_BOX;
	//			}
	//		}

	//	}
	//}

	//// clean up collision events
	//for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}
