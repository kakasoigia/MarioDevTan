#include "FireBullet.h"
 CFireBullet::CFireBullet() 
{
	isUsed = false;
	state == BULLET_STATE_DISAPPEARING;
}
void CFireBullet::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{	// if it's not fired ...return
	if (!isUsed) return;
	// Calculate dx, dy 
	CGameObject::Update(dt);
	
	// Simple fall down
	vy += BULLET_GRAVITY * dt;
	//set speed//
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	// turn off collision when die 
	/*if (state != BULLET_STATE_DISAPPEARING)*/
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	/*if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}*/

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

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 
		// block every object first!
		/*x += min_tx * dx + nx * 0.4f;*/
		y += min_ty * dy + ny * 0.4f;
		/*if (nx != 0) vx = 0;*/
		if (ny > 0) vy = 0.2f;
		else if (ny < 0) vy = -0.2f;



		//
		// Collision logic with other objects

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CKoopas *>(e->obj)) // if e->obj is Goomba 
			{
				CKoopas *koopas = dynamic_cast<CKoopas *>(e->obj);
				if (nx != 0 )
				{
					koopas->SetState(KOOPAS_STATE_DIE);
					SetState(BULLET_STATE_DISAPPEARING);
				}
				
			}
			else if(dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);
				if (nx != 0)
				{
					goomba->SetState(GOOMBA_STATE_DIE);
					SetState(BULLET_STATE_DISAPPEARING);
				}

			}
			else if (dynamic_cast<CPipe *>(e->obj) || dynamic_cast<CBrick *>(e->obj) || x<0 || y<100 )// hit other things
			{
				if (e->nx != 0 && ny == 0)
				{
					SetState(BULLET_STATE_DISAPPEARING);
				}
			}
			else 
			{

			}
		}

	}


	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void CFireBullet::Render()
{
	if (!isUsed) return;// if not used ,dont render;
	int ani = -1;
	if (nx > 0) ani = 1;
	else ani = 0;
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void CFireBullet::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BULLET_BBOX_WIDTH;
	b = y + BULLET_BBOX_HEIGHT;
}
void  CFireBullet::SetState(int state)
{
	if (state == BULLET_STATE_FLYING)
	{
		CMario *mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		this->isUsed = true;
		this->state = BULLET_STATE_FLYING;
		this->x = mario->x + 5;
		this->y = mario->y+2;
		this->nx = mario->nx;
		if (nx > 0)
			vx = BULLET_FLYING_SPEED;
		else
			vx = -BULLET_FLYING_SPEED;
		/*vy = BULLET_FLYING_SPEED /5;*/
	}
	else if (state == BULLET_STATE_DISAPPEARING)
	{
		isUsed = false;
		SetPosition(-1000, -1000); // hidden bullet
	}
		
}