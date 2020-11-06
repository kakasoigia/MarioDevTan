#include "Koopas.h"
#include "Brick.h"
CKoopas::CKoopas(int type)
{
	Type = type;
	nx = -1;
	SetState(KOOPAS_STATE_WALKING);
	flying_start = 0;
}

void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == KOOPAS_STATE_DIE)
		return;
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;
	if (state == KOOPAS_STATE_SHELL || state == KOOPAS_STATE_SPINNING)
	{
		bottom = y + KOOPAS_BBOX_HEIGHT_SHELL;
	}
	else if (state == KOOPAS_STATE_DIE)
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);
	if (isHolding != KOOPAS_HOLDING)
		vy += KOOPAS_GRAVITY * dt;
	if (this->state == KOOPAS_STATE_WALKING && (this->Type == KOOPAS_TYPE_GREEN_FLY || this->Type == KOOPAS_TYPE_RED_FLY))
	{
		if (GetTickCount() - flying_start > KOOPAS_PERIODIC_TIME_FLY)
		{
			vy = -KOOPAS_FLY_DEFLECT_SPEED;
			flying_start = GetTickCount();
		}
	}


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	//// turn off collision when die 
	//if (state == GOOMBA_STATE_DIE)
	//{
	//	if ((GetTickCount() - dying_start) > GOOMBA_DYING_TIME)
	//	{
	//		dying_start = 0;
	//		state = GOOMBA_STATE_DISAPPEAR;
	//	}
	//}
	//shell is being held
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (state != KOOPAS_STATE_DIE)
	{

		CalcPotentialCollisions(coObjects, coEvents);
	}
	if (mario->GetIsHolding() == 0) // throw and kick Koo when drop Koo
	{
		isHolding = KOOPAS_NOT_HOLDING;
		/*mario->StartKicking();
		mario->SetIsKicking(true);
		nx = mario->nx;
		SetState(KOOPAS_STATE_SPINNING);*/
	}
	else if (isHolding == KOOPAS_HOLDING)
	{
		y = mario->y + 8;
		if (mario->nx > 0)
		{
			if (mario->GetLevel() == MARIO_LEVEL_BIG)
			{
				x = mario->x + MARIO_BIG_BBOX_WIDTH;
			}
			else if (mario->GetLevel() == MARIO_LEVEL_SMALL)
			{
				x = mario->x + MARIO_SMALL_BBOX_WIDTH - 1;
				y = y - 10;
			}
			else if (mario->GetLevel() == MARIO_LEVEL_TAIL)
			{
				x = mario->x + MARIO_TAIL_BBOX_WIDTH;
			}
			else
			{
				x = mario->x + MARIO_FIRE_BBOX_WIDTH;
			}
		}
		else
		{
			if (mario->GetLevel() == MARIO_LEVEL_BIG)
			{
				x = mario->x - MARIO_BIG_BBOX_WIDTH;
			}
			else if (mario->GetLevel() == MARIO_LEVEL_SMALL)
			{
				x = mario->x - MARIO_SMALL_BBOX_WIDTH + 1;
				y = y - 10;
			}
			else if (mario->GetLevel() == MARIO_LEVEL_TAIL)
			{
				x = mario->x - MARIO_TAIL_BBOX_WIDTH;
			}
			else
			{
				x = mario->x - MARIO_FIRE_BBOX_WIDTH;
			}
		}
	}



	// reset untouchable timer if untouchable time has passed

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

	}
	else
	{
		// land ...fly
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block 
		/*if (!isHolding)
			x += min_tx * dx + nx * 0.4f;*/			// nx*0.4f : need to push out a bit to avoid overlapping next frame
			/*y += min_ty * dy + ny * 0.4f;*/

			/*if (nx != 0) vx = -vx;*/
		if (ny != 0) vy = 0;

		// Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);
				if (this->state == KOOPAS_STATE_WALKING || this->state == KOOPAS_STATE_SHELL)
				{
					this->vx = -this->vx;
					goomba->vx = -goomba->vx;
				}
				else if (e->nx != 0 && (this->state == KOOPAS_STATE_SPINNING || isHolding == KOOPAS_HOLDING))
				{
					goomba->SetState(GOOMBA_STATE_DIE);
				}

			}
			else if (dynamic_cast<CKoopas *>(e->obj)) // if e->obj is Goomba 
			{
				CKoopas *koopas = dynamic_cast<CKoopas *>(e->obj);
				if (nx != 0 && (this->state == KOOPAS_STATE_SPINNING || isHolding == KOOPAS_HOLDING))
				{
					koopas->SetState(KOOPAS_STATE_DIE);
				}
				else if (this->state == KOOPAS_STATE_WALKING)
				{
					this->vx = -this->vx;
				}
			}
			else if (!dynamic_cast<CMario *>(e->obj))
			{
				if (e->nx != 0 && ny == 0)
				{
					this->vx = -this->vx;
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	if (x <= 0)
		if (vx < 0)
			vx = -vx;
}

void CKoopas::Render()
{
	int ani = KOOPAS_ANI_GREEN_WALKING_LEFT;
	// xét trạng thái WALKING hay MAI RÙA
	if (state == KOOPAS_STATE_WALKING)
	{
		switch (Type)
		{
		case 1 /*KOOPAS_TYPE_GREEN_WALK */:
		{
			if (vx > 0) ani = KOOPAS_ANI_GREEN_WALKING_RIGHT;
			else if (vx <= 0) ani = KOOPAS_ANI_GREEN_WALKING_LEFT;
		}
		break;
		case 2 /*KOOPAS_TYPE_GREEN_FLY*/:
		{
			ani = KOOPAS_ANI_GREEN_FLYING_LEFT;
		}
		break;
		case 3 /*KOOPAS_TYPE_RED_WALK*/:
		{
			if (vx > 0) ani = KOOPAS_ANI_RED_WALKING_RIGHT;
			else if (vx <= 0) ani = KOOPAS_ANI_RED_WALKING_LEFT;
			
		}
		break;
		case 4 /*KOOPAS_TYPE_RED_FLY*/:
		{
			ani = KOOPAS_ANI_RED_FLYING_LEFT;
		}
		break;
		}
	}
	else if (state == KOOPAS_STATE_SHELL)
	{
		// chưa xử lý được chết úp hay ngửa
		switch (Type)
		{
		case 1 /*KOOPAS_TYPE_GREEN_WALK */:
		{
			ani = KOOPAS_ANI_GREEN_SHELL_PRONE;
		}
		break;
		case 2 /*KOOPAS_TYPE_GREEN_FLY*/:
		{
			ani = KOOPAS_ANI_GREEN_SHELL_PRONE;
		}
		break;
		case 3 /*KOOPAS_TYPE_RED_WALK*/:
		{
			ani = KOOPAS_ANI_RED_SHELL_PRONE;
		}
		break;
		case 4 /*KOOPAS_TYPE_RED_FLY*/:
		{
			ani = KOOPAS_ANI_RED_SHELL_PRONE;
		}
		break;
		}
	}
	else if (state == KOOPAS_STATE_SPINNING)
	{
		// chưa xử lý được chết úp hay ngửa
		switch (Type)
		{
		case 1 /*KOOPAS_TYPE_GREEN_WALK */:
		{
			ani = KOOPAS_ANI_GREEN_SPINNING;
		}
		break;
		case 2 /*KOOPAS_TYPE_GREEN_FLY*/:
		{
			ani = KOOPAS_ANI_GREEN_SPINNING;
		}
		break;
		case 3 /*KOOPAS_TYPE_RED_WALK*/:
		{
			ani = KOOPAS_ANI_RED_ANI_SPINNING;
		}
		break;
		case 4 /*KOOPAS_TYPE_RED_FLY*/:
		{
			ani = KOOPAS_ANI_RED_ANI_SPINNING;
		}
		break;
		}
	}
	else if (state == KOOPAS_STATE_DIE)
	{
		// chưa xử lý được chết úp hay ngửa
		switch (Type)
		{
		case 1 /*KOOPAS_TYPE_GREEN_WALK */:
		{
			ani = KOOPAS_ANI_GREEN_SHELL_UPWARD;
		}
		break;
		case 2 /*KOOPAS_TYPE_GREEN_FLY*/:
		{
			ani = KOOPAS_ANI_GREEN_SHELL_UPWARD;
		}
		break;
		case 3 /*KOOPAS_TYPE_RED_WALK*/:
		{
			ani = KOOPAS_ANI_RED_SHELL_PRONE;
		}
		break;
		case 4 /*KOOPAS_TYPE_RED_FLY*/:
		{
			ani = KOOPAS_ANI_RED_SHELL_PRONE;
		}
		break;
		}
	}

	animation_set->at(ani)->Render(x, y);


}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_SHELL:
		/*y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE + 1;*/
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_WALKING:
		if (nx > 0)
		{
			vx = KOOPAS_WALKING_SPEED;
		}
		else if (nx < 0)
		{
			vx = -KOOPAS_WALKING_SPEED;
		}
		break;
	case KOOPAS_STATE_SPINNING:
		if (nx > 0)
		{
			vx = KOOPAS_SPINNING_SPEED;
		}
		else if (nx < 0)
		{
			vx = -KOOPAS_SPINNING_SPEED;
		}
		break;
	case KOOPAS_STATE_DIE:
		vx = -vx;
		vy -= KOOPAS_DIE_DEFLECT_SPEED;
		break;
	}

}