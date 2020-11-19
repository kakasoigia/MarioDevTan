#include "Goomba.h"

#define GOOMBA_GRAVITY 0.002f

CGoomba::CGoomba(int type)
{
	Type = type;
	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == GOOMBA_STATE_DIE_BY_KICK || state == GOOMBA_STATE_DIE)
	{
		left = top = right = bottom = 0;
		return;
	}
	left = x;
	top = y;
	right = x + GOOMBA_NORMAL_BBOX_WIDTH;
	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else
		bottom = y + GOOMBA_NORMAL_BBOX_HEIGHT;
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 
	if (GetTickCount() - jumping_start >= GOOMBA_TIME_JUMPING && Type == GOOMBA_TYPE_RED_FLY) // GOOMBA RED FLY JUMP
	{

		vy = -GOOMBA_JUMP_SPEED;
		jumping_start = GetTickCount();

	}



	// Simple fall down
	vy += GOOMBA_GRAVITY * dt;
	if (this->state == GOOMBA_STATE_WALKING && this->Type == GOOMBA_TYPE_RED_FLY)
	{
		if (GetTickCount() - jumping_start > GOOMBA_PERIODIC_TIME_JUMPING)
		{
			vy = -GOOMBA_JUMP_DEFLECT_SPEED;
				jumping_start = GetTickCount();
		}
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state == GOOMBA_STATE_DIE || GOOMBA_STATE_DIE_BY_KICK)
	{
		if ((GetTickCount() - dying_start) > GOOMBA_DYING_TIME)
		{
			dying_start = 0;
			
		}
	}

	if (state != GOOMBA_STATE_DIE_BY_KICK && state != GOOMBA_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);



	// reset untouchable timer if untouchable time has passed

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		//// block 
		//x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		//y += min_ty * dy + ny * 0.4f;

		/*if (nx != 0) vx = -vx;*/
		if (ny != 0) vy = 0;

		// Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);
				this->vx = -this->vx;
				goomba->vx = -goomba->vx;


			}
			//else if ( dynamic_cast<CPipe *>(e->obj)) /*dynamic_cast<CBrick *>(e->obj) ||*/
			{
				
				if (e->nx !=0 && ny ==0)
				{
					this->vx = -this->vx;
					
				}
					
			}
			
			//else if (dynamic_cast<CBrick *>(e->obj)) // if e->obj is Brick 
			//{
			//	if (e->ny < 0)
			//	{
			//		// chạm gạch dưới ...do nothing
			//	}
			//	
			//}
			//else
			//{
			//	
			//}
			if (vx < 0 && x <= 0) 
			{
				x = 0;
				vx = -vx;
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	//// move border
	//if (vx < 0 && x < 0) {
	//	x = 0; vx = -vx;
	//}

	//if (vx > 0 && x > 290) {
	//	x = 290; vx = -vx;
	//}

}

void CGoomba::Render()
{
	int ani = -1;
	if (state == GOOMBA_STATE_WALKING || state == GOOMBA_STATE_DIE_BY_KICK)
	{
		// chưa xử lý được chết úp hay ngửa
		switch (Type)
		{
		case 1 /*GOOMBA_TYPE_ORANGE */:
		{
			ani = GOOMBA_ANI_ORANGE_WALKING;
		}
		break;
		case 2 /*GOOMBA_TYPE_RED_FLY*/:
		{
			ani = GOOMBA_ANI_RED_FLY_WALKING;
		}
		break;
		case 3: //GOOMBA_TYPE_RED_WALK
		{
			ani = GOOMBA_ANI_RED_WALKING;
		}
		break;
		}
	}
	else if (state == GOOMBA_STATE_DIE )
	{
		switch (Type)
		{
		case 1 /*GOOMBA_TYPE_ORANGE */:
		{
			ani = GOOMBA_ANI_ORANGE_DIE;
		}
		break;
		case 2 /*GOOMBA_TYPE_RED_FLY*/: case 3:
		{
			ani = GOOMBA_ANI_RED_DIE;
		}
		break;

		}
	}
	
	else if (state == GOOMBA_STATE_DISAPPEAR)
	{

		return;
	}

	animation_set->at(ani)->Render(x, y);
	/*RenderBoundingBox();*/
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		y += GOOMBA_NORMAL_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case GOOMBA_STATE_DIE_BY_KICK:
		vy = -GOOMBA_DIE_DEFLECT_SPEED;
		vx = -vx;
		break;

	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
		break;
	}
}
