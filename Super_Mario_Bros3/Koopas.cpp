#include "Koopas.h"
#include "Brick.h"
CKoopas::CKoopas(int type)
{
	Type = type;

	SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_DIE)
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);
	vy += KOOPAS_GRAVITY * dt;

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
	if (state != KOOPAS_STATE_DISAPPEAR)
	{

		CalcPotentialCollisions(coObjects, coEvents);
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
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		/*if (nx != 0) vx = -vx;*/
		if (ny != 0) vy = 0;

		// Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			//if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			//{
			//	CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);
			//	this->vx = -this->vx;
			//	goomba->vx = -goomba->vx;
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CKoopas::Render()
{
	int ani = KOOPAS_ANI_GREEN_WALK_WALKING_LEFT;
	// xét trạng thái WALKING hay MAI RÙA
	if (state == KOOPAS_STATE_WALKING)
	{
		switch (Type)
		{
		case 1 /*KOOPAS_TYPE_GREEN_WALK */:
		{
			if (vx > 0) ani = KOOPAS_ANI_GREEN_WALK_WALKING_RIGHT;
			else if (vx <= 0) ani = KOOPAS_ANI_GREEN_WALK_WALKING_LEFT;
		}
		break;
		case 2 /*KOOPAS_TYPE_GREEN_FLY*/:
		{
			ani = KOOPAS_ANI_GREEN_FLY_WALKING_LEFT;
		}
		break;
		case 3 /*KOOPAS_TYPE_RED_WALK*/:
		{
			ani = KOOPAS_ANI_RED_WALK_WALKING_LEFT;
		}
		break;
		case 4 /*KOOPAS_TYPE_RED_FLY*/:
		{
			ani = KOOPAS_ANI_RED_FLY_WALKING_LEFT;
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
			ani = KOOPAS_ANI_RED_FLY_SHELL_UP;
		}
		break;
		case 4 /*KOOPAS_TYPE_RED_FLY*/:
		{
			ani = KOOPAS_ANI_RED_FLY_SHELL_UP;
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
	case KOOPAS_STATE_DIE:
		/*y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE + 1;*/
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_WALKING:
		vx = -KOOPAS_WALKING_SPEED;

	}

}