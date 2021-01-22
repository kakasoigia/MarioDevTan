#include "FireBullet.h"
#include "HitEffect.h"
#include "BoomerangEnemy.h"
 CFireBullet::CFireBullet() 
{
	isUsed = false;
	state = BULLET_STATE_DISAPPEARING;
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
			CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CKoopas *>(e->obj)) // if e->obj is Goomba 
			{
				CKoopas *koopas = dynamic_cast<CKoopas *>(e->obj);
				if (nx != 0 )
				{
					mario->IncScore(100, koopas->x, koopas->y);
					mario->CallHitEffect(HIT_EFFECT_FIRE_BULLET, this->nx, this->x, this->y);
					koopas->SetState(KOOPAS_STATE_DIE);
					SetState(BULLET_STATE_DISAPPEARING);
					
					
				}
				
			}
			else if(dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);
				if (nx != 0)
				{
					mario->IncScore(100, goomba->x, goomba->y);
					mario->CallHitEffect(HIT_EFFECT_FIRE_BULLET, this->nx, this->x, this->y);
					goomba->SetState(GOOMBA_STATE_DIE_BY_KICK);
					SetState(BULLET_STATE_DISAPPEARING);
				
					
				}

			}
			else if (dynamic_cast<CBoomerangEnemy*>(e->obj)) // if e->obj is Goomba 
			{
				CBoomerangEnemy *boomerangEnemy = dynamic_cast<CBoomerangEnemy *>(e->obj);
				if (nx != 0)
				{
					mario->CallHitEffect(HIT_EFFECT_FIRE_BULLET, 1, this->x, this->y);
					boomerangEnemy->SetState(BOOMERANG_ENEMY_STATE_DIE);
					mario->IncScore(100, boomerangEnemy->x, boomerangEnemy->y);

					SetState(BULLET_STATE_DISAPPEARING);
					
					
				}

			}
			else if (dynamic_cast<CPipe *>(e->obj) || dynamic_cast<CBrick *>(e->obj) || dynamic_cast<CBreakableBrick *>(e->obj) )// hit other things
			{
				if (e->nx != 0 && ny == 0)
				{
					mario->CallHitEffect(HIT_EFFECT_FIRE_BULLET, 1, this->x, this->y);
					SetState(BULLET_STATE_DISAPPEARING);
					
				}
			}
			else 
			{
				mario->CallHitEffect(HIT_EFFECT_FIRE_BULLET, 1, this->x, this->y);
					SetState(BULLET_STATE_DISAPPEARING);
			}
			
		}

	}
	if (this->x < 0 || this->y >170)
	{
		SetState(BULLET_STATE_DISAPPEARING);
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
		SetPosition(1000, 1000); // hidden bullet
	}
		
}