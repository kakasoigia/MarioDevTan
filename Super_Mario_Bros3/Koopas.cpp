#include "Koopas.h"
#include "Brick.h"
#include "BreakableBrick.h"
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
void CKoopas::FilterCollision(vector<LPCOLLISIONEVENT> &coEvents, vector<LPCOLLISIONEVENT> &coEventsResult, float &min_tx, float &min_ty, float &nx, float &ny, float &rdx, float &rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
		}

		if (c->t < min_ty  && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
		}
		if (dynamic_cast<CMario *>(c->obj))
		{
			ny = -0.01f;
		}
	}
	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}


void CKoopas::CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		//CMario* player = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		if (dynamic_cast<CRectangle *>(coObjects->at(i)) && vy < 0)
		{
			continue;
		}
		if (dynamic_cast<CFlowerBullet *>(coObjects->at(i)))
		{
			continue;
		}
		if (dynamic_cast<CBreakableBrick *>(coObjects->at(i)))
		{

			CBreakableBrick *breakable_brick = dynamic_cast<CBreakableBrick *>(coObjects->at(i));
			if (breakable_brick->GetState() != BREAKABLE_STATE_SHOW)
			{
				continue;
			}
		}
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	
	CGameObject::Update(dt, coObjects);
	if (!isHolding)
		vy += KOOPAS_GRAVITY * dt;

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (this->state == KOOPAS_STATE_WALKING && (this->Type == KOOPAS_TYPE_GREEN_FLY || this->Type == KOOPAS_TYPE_RED_FLY))
	{
		if (GetTickCount() - flying_start > KOOPAS_PERIODIC_TIME_FLY)
		{
			vy = -KOOPAS_FLY_DEFLECT_SPEED;
			flying_start = GetTickCount();
		}
	}
	if (GetTickCount() - reviveStart >= KOOPAS_TIME_REVIVE)
	{
		if (state == KOOPAS_STATE_SHELL)
		{
			y -= 10;
			x += 5 * mario->nx;
			SetState(KOOPAS_STATE_WALKING);
			if (mario->x >= this->x)
				vx = -vx;
			isHolding = false;
			mario->SetIsHolding(false);
			reviveRender = false;
		}
		reviveStart = 0;
	}
	else
	{
		if (GetTickCount() - reviveStart >= KOOPAS_TIME_ABOUT_TO_REVIVE)
		{
			reviveRender = true;
			
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
	
	if (state != KOOPAS_STATE_DIE)
	{

		CalcPotentialCollisions(coObjects, coEvents);
	}
	if (isHolding == true)
	{
		if (!mario->GetIsHolding())
		{
			isHolding = false;
			mario->StartKicking();
			mario->SetIsKicking(true);
			mario->SetIsHoldAni(false);
			nx = mario->nx;
			SetState(KOOPAS_STATE_SPINNING);
		}
	}
	 if (isHolding )
	{
		y = mario->y + 8;
		if (mario->nx > 0) //turn right
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
		else// go left
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
				x = mario->x - MARIO_TAIL_BBOX_WIDTH +6;
			}
			else
			{
				x = mario->x - MARIO_FIRE_BBOX_WIDTH;
			}
		}
		mario->SetIsHoldAni(true);
	}



	// reset untouchable timer if untouchable time has passed
	 if (state != KOOPAS_STATE_WALKING)
		 CanPullBack = false;
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		if (CanPullBack && Type == KOOPAS_TYPE_RED_WALK)
		{
			if (y - CheckPosition_Y >= 1.0f)
			{

				y -= 3;
				if (vx < 0)
					x += 12;
				else
					x -= 12;
				vx = -vx;
			}
		}
	}
	else
	{
		// land ...fly
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		if (!isHolding)
			x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		/*y += min_ty * dy + ny * 0.4f;*/

		if (ny != 0) vy = 0;
		if (ny < 0 && state == KOOPAS_STATE_SHELL)
		{
			vx = 0;
		}
	
		// Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (!dynamic_cast<CMario *>(e->obj) && nx == 0)
			{
				CheckPosition_Y = y;
				CanPullBack = true;
			}
			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);
				if (this->state == KOOPAS_STATE_WALKING || this->state == KOOPAS_STATE_SHELL)
				{
					this->vx = -this->vx;
					goomba->vx = -goomba->vx;
				}
				else if (e->nx != 0 && (this->state == KOOPAS_STATE_SPINNING || isHolding == true))
				{
					goomba->SetState(GOOMBA_STATE_DIE_BY_KICK);
				}

			}
			else if (dynamic_cast<CQuestionBrick *>(e->obj))
			{
				//if (e->nx != 0/* && ny == 0*/)
				//{

				CQuestionBrick *question_brick = dynamic_cast<CQuestionBrick *>(e->obj);
				if (state == KOOPAS_STATE_SPINNING)
				{
					if (question_brick->GetIsAlive())
					{
						question_brick->SetState(QUESTION_BRICK_STATE_USED, coObjects);
						question_brick->SetIsUp(true);
					}
						
					vx = -vx;
				}

				/*}*/

			}
			else if (dynamic_cast<CKoopas *>(e->obj)) // if e->obj is Goomba 
			{
				CKoopas *koopas = dynamic_cast<CKoopas *>(e->obj);
				if (nx != 0 && (this->state == KOOPAS_STATE_SPINNING || isHolding == true))
				{
					koopas->SetState(KOOPAS_STATE_DIE);
				}
				
			}
			else if (dynamic_cast<CBreakableBrick *>(e->obj))
			{
				CBreakableBrick *brick = dynamic_cast<CBreakableBrick *>(e->obj);
				if (e->nx != 0 && ny == 0 && this->state == KOOPAS_STATE_SPINNING)
				{
					this->vx = -this->vx;
					brick->SetState(BREAKABLE_STATE_BROKEN);

				}
				if (e->ny < 0 && brick->GetIsBouncing() )
				{
					this->state = KOOPAS_STATE_SHELL;
					vy -= 0.1f;
				}
			}
			else if (!dynamic_cast<CMario *>(e->obj))
			{
				if (e->nx != 0 && ny == 0)
				{
					this->vx = -this->vx;
				}
				/*	if (e->ny!=0) canTurn = true;
					else canTurn = false;
				}*/
			}
		}
	}
	/*if (Type == KOOPAS_TYPE_RED_WALK)
	{
		if (((this->x <= 490 || this->x >= 595) && (this->x <= 660 && this->x >= 480)) || ((this->x <= 2078 || this->x >= 2099) && (this->x <= 2212 && this->x >= 1960)))
		{
			if (state == KOOPAS_STATE_WALKING)
				vx = -vx;
		}

	}*/
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
			if (reviveRender) ani = KOOPAS_XANH_ANI_REVIVING;
			else 
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
		StartTimeRevive();
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