#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"
#include "Rectangle.h"
#include "Goomba.h"
#include "Portal.h"
#include "Koopas.h"

CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_BIG;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	current_level_speed_up = 0;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	if (state != MARIO_STATE_FLY && state != MARIO_STATE_FALL_DOWN)
		vy += MARIO_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	if (GetTickCount() - kicking_start > MARIO_KICK_TIME)
	{
		kicking_start = 0;
		isKicking = false;
	}
	if (GetTickCount() - turning_start > MARIO_TURNING_TIME)
	{
		isTurning = false;
	}
	
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
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		/*if (nx != 0) vx = 0;*/
		if (ny != 0) vy = 0;
		// check if can fly again?
		if (ny < 0)
		{
			isJumping = false;
			isLanding = false;
			canFly = true;
		}

		/*if (coEventsResult.size() == 0) isJumping = true;*/

		//
		// Collision logic with other objects

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			//if (e->ny < 0) isJumping = false; // chạm vật có thể nhảy tiếp
			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					/*if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						goomba->StartDyingTime();
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}*/
					if (goomba->GetState() == GOOMBA_TYPE_RED_FLY)
					{
						goomba->SetState(GOOMBA_TYPE_RED_WALK);
					}
					else
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						goomba->StartDyingTime();
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0 || e->ny > 0)
				{
					if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								DebugOut(L"[INFO] Lỗi untouch \n");
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
					else if (level == MARIO_LEVEL_TAIL && isTurning)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE_BY_KICK)
							goomba->SetState(GOOMBA_STATE_DIE_BY_KICK);
					}
				}

			}
			else if (dynamic_cast<CKoopas *>(e->obj))
			{
				CKoopas *koopas = dynamic_cast<CKoopas *>(e->obj);
				if (e->ny < 0)
				{
					if (koopas->GetState() != KOOPAS_STATE_DIE && koopas->GetState() != KOOPAS_STATE_SHELL)
					{
						if (koopas->GetType() == KOOPAS_TYPE_GREEN_FLY || koopas->GetType() == KOOPAS_TYPE_RED_FLY)
						{
							koopas->SetType(koopas->GetType()-1); //subtract 1 type from fly to walk
						}
						else
							koopas->SetState(KOOPAS_STATE_SHELL);
						vy = -2*MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (koopas->GetState() == KOOPAS_STATE_SHELL)
					{
						if (ny == 0) // kick Koo || hold Koo
						{
							if (isHolding == 1/*; state == MARIO_STATE_RUNNING_LEFT || state == MARIO_STATE_RUNNING_RIGHT*/)
							{
								
								koopas->SetIsHolding(KOOPAS_HOLDING);
							}
							else
							{
								isHolding == -1;
								isKicking = true;
								StartKicking();
								koopas->nx = this->nx;
								koopas->SetState(KOOPAS_STATE_SPINNING);
								/*if (this->nx > 0)
								{
									koopas->nx = 1;
									koopas->SetState(KOOPAS_STATE_SPINNING);
								}
								else if (this->nx < 0)
								{
									koopas->nx = -1;
									koopas->SetState(KOOPAS_STATE_SPINNING);
								}*/
							}
						}
					}
					else if (koopas->GetState() != KOOPAS_STATE_SHELL && isKicking == false)
					{
						if (untouchable == 0)
						{	DebugOut(L"[INFO] lỗi 1\n");
							if (koopas->GetState() != KOOPAS_STATE_DIE)
							{
								if (level > MARIO_LEVEL_SMALL)
								{
									level = MARIO_LEVEL_SMALL;
									StartUntouchable();
								}
								else
									SetState(MARIO_STATE_DIE);
							}
						}
					}
				}

			}
			else if (dynamic_cast<CPortal *>(e->obj))
			{
				CPortal *p = dynamic_cast<CPortal *>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
		}

	}


	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CMario::Render()
{
	int ani = -1;
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	else if (state == MARIO_STATE_FLY)
	{
		
		 if (level == MARIO_LEVEL_TAIL)
		{
			if (nx > 0) ani = MARIO_ANI_TAIL_FLYING_RIGHT;
			else ani = MARIO_ANI_TAIL_FLYING_LEFT;
		}
		
	}
	else if (isTurning)
	{
		if (nx < 0) ani = MARIO_ANI_TAIL_TURNING_RIGHT;
		else ani = MARIO_ANI_TAIL_TURNING_LEFT;
	}
	else if (isFiring)
	{
		if (nx < 0) ani = MARIO_ANI_TAIL_FIRING_BULLET_LEFT;
		else ani = MARIO_ANI_TAIL_FIRING_BULLET_RIGHT;
	}
	else if (isJumping == true) // đang trên kh ?
	{
		if (level == MARIO_LEVEL_BIG)
		{
			if (nx > 0)
			{
				ani = MARIO_ANI_BIG_JUMPING_RIGHT;
			}
			else
			{
				ani = MARIO_ANI_BIG_JUMPING_LEFT;
			}
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			if (nx > 0)
			{
				ani = MARIO_ANI_SMALL_JUMPING_RIGHT;
			}
			else
			{
				ani = MARIO_ANI_SMALL_JUMPING_LEFT;
			}
		}
		else if (level == MARIO_LEVEL_TAIL)
		{
			if (nx > 0)
			{
				ani = MARIO_ANI_TAIL_JUMPING_RIGHT;
			}
			else
			{
				ani = MARIO_ANI_TAIL_JUMPING_LEFT;
			}
		}
		else if (level == MARIO_LEVEL_FIRE)
		{
			if (nx > 0)
			{
				ani = MARIO_ANI_FIRE_JUMPING_RIGHT;
			}
			else
			{
				ani = MARIO_ANI_FIRE_JUMPING_LEFT;
			}
		}
	}
	else if (isHolding ==1)
	{
		if (level == MARIO_LEVEL_BIG)
		{
			if (nx > 0) ani = MARIO_ANI_BIG_HOLDING_RIGHT;
			else ani = MARIO_ANI_BIG_HOLDING_LEFT;
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			if (nx > 0) ani = MARIO_ANI_SMALL_HOLDING_RIGHT;
			else ani = MARIO_ANI_SMALL_HOLDING_LEFT;
		}
		else if (level == MARIO_LEVEL_TAIL)
		{
			if (nx > 0) ani = MARIO_ANI_TAIL_HOLDING_RIGHT;
			else ani = MARIO_ANI_TAIL_HOLDING_LEFT;
		}
		else if (level == MARIO_LEVEL_FIRE)
		{
			if (nx > 0) ani = MARIO_ANI_FIRE_HOLDING_RIGHT;
			else ani = MARIO_ANI_FIRE_HOLDING_LEFT;
		}
	}
	

	else if (state == MARIO_STATE_IDLE)
	{
		if (level == MARIO_LEVEL_BIG)
		{
			if (nx > 0) ani = MARIO_ANI_BIG_IDLE_RIGHT;
			else ani = MARIO_ANI_BIG_IDLE_LEFT;
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
			else ani = MARIO_ANI_SMALL_IDLE_LEFT;
		}
		else if (level == MARIO_LEVEL_TAIL)
		{
			if (nx > 0) ani = MARIO_ANI_TAIL_IDLE_RIGHT;
			else ani = MARIO_ANI_TAIL_IDLE_LEFT;
		}
		else if (level == MARIO_LEVEL_FIRE)
		{
			if (nx > 0) ani = MARIO_ANI_FIRE_IDLE_RIGHT;
			else ani = MARIO_ANI_FIRE_IDLE_LEFT;
		}
	}
	else if (state == MARIO_STATE_SITDOWN)
	{
		if (level == MARIO_LEVEL_BIG)
		{
			if (nx > 0) ani = MARIO_ANI_BIG_SITDOWN_RIGHT;
			else ani = MARIO_ANI_BIG_SITDOWN_LEFT;
		}
		else if (level == MARIO_LEVEL_TAIL)
		{
			if (nx > 0) ani = MARIO_ANI_TAIL_SITDOWN_RIGHT;
			else ani = MARIO_ANI_TAIL_SITDOWN_LEFT;
		}
		else if (level == MARIO_LEVEL_FIRE)
		{
			if (nx > 0) ani = MARIO_ANI_FIRE_SITDOWN_RIGHT;
			else ani = MARIO_ANI_FIRE_SITDOWN_LEFT;
		}
		else //nhỏ
		{
			if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
			else ani = MARIO_ANI_SMALL_IDLE_RIGHT;
		}
	}
	else if (isBraking)
	{
		if (level == MARIO_LEVEL_BIG)
		{
			if (nx > 0) ani = MARIO_ANI_BIG_BRAKING_RIGHT;
			else ani = MARIO_ANI_BIG_BRAKING_LEFT;
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			if (nx > 0) ani = MARIO_ANI_SMALL_BRAKING_RIGHT;
			else ani = MARIO_ANI_SMALL_BRAKING_LEFT;
		}
		else if (level == MARIO_LEVEL_TAIL)
		{
			if (nx > 0) ani = MARIO_ANI_TAIL_BRAKING_RIGHT;
			else ani = MARIO_ANI_TAIL_BRAKING_LEFT;
		}
		else if (level == MARIO_LEVEL_FIRE)
		{
			if (nx > 0) ani = MARIO_ANI_FIRE_BRAKING_RIGHT;
			else ani = MARIO_ANI_FIRE_BRAKING_LEFT;
		}
	}
	else if (state == MARIO_STATE_RUNNING_RIGHT)
	{
		if (level == MARIO_LEVEL_BIG)
		{
			ani = MARIO_ANI_BIG_RUNNING_RIGHT;

		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			ani = MARIO_ANI_SMALL_RUNNING_RIGHT;

		}
		else if (level == MARIO_LEVEL_TAIL)
		{
			ani = MARIO_ANI_TAIL_RUNNING_RIGHT;

		}
		else if (level == MARIO_LEVEL_FIRE)
		{
			ani = MARIO_ANI_FIRE_RUNNING_RIGHT;

		}
	}
	else if (state == MARIO_STATE_RUNNING_LEFT)
	{
		if (level == MARIO_LEVEL_BIG)
		{
			ani = MARIO_ANI_BIG_RUNNING_LEFT;
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			ani = MARIO_ANI_SMALL_RUNNING_LEFT;
		}
		else if (level == MARIO_LEVEL_TAIL)
		{
			ani = MARIO_ANI_TAIL_RUNNING_LEFT;
		}
		else if (level == MARIO_LEVEL_FIRE)
		{
			ani = MARIO_ANI_FIRE_RUNNING_LEFT;
		}
	}
	
	
	
	else if (isKicking == true)
	{
	if (level == MARIO_LEVEL_BIG)
	{
		if (nx > 0) ani = MARIO_ANI_BIG_KICKING_RIGHT;
		else ani = MARIO_ANI_BIG_KICKING_LEFT;
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		if (nx > 0) ani = MARIO_ANI_SMALL_KICKING_RIGHT;
		else ani = MARIO_ANI_SMALL_KICKING_LEFT;
	}
	else if (level == MARIO_LEVEL_TAIL)
	{
		if (nx > 0) ani = MARIO_ANI_TAIL_KICKING_RIGHT;
		else ani = MARIO_ANI_TAIL_KICKING_LEFT;
	}
	else if (level == MARIO_LEVEL_FIRE)
	{
		if (nx > 0) ani = MARIO_ANI_FIRE_KICKING_RIGHT;
		else ani = MARIO_ANI_FIRE_KICKING_LEFT;
	}
	}
	
	
	else if (nx > 0) // walking right
	{
		if (level == MARIO_LEVEL_BIG)
		{
				ani = MARIO_ANI_BIG_WALKING_RIGHT;
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
				ani = MARIO_ANI_SMALL_WALKING_RIGHT;
		}
		else if (level == MARIO_LEVEL_TAIL)
		{
			ani = MARIO_ANI_TAIL_WALKING_RIGHT;

		}
		else if (level == MARIO_LEVEL_FIRE)
		{
			ani = MARIO_ANI_FIRE_WALKING_RIGHT;

		}

	}

	else if (nx < 0) // walking left
	{
		if (level == MARIO_LEVEL_BIG)
		{
				ani = MARIO_ANI_BIG_WALKING_LEFT;

		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			ani = MARIO_ANI_SMALL_WALKING_LEFT;

		}
		else if (level == MARIO_LEVEL_TAIL)
		{

			ani = MARIO_ANI_TAIL_WALKING_LEFT;
		}
		else if (level == MARIO_LEVEL_FIRE)
		{

			ani = MARIO_ANI_FIRE_WALKING_LEFT;
		}
	}
	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y, alpha);

	/*RenderBoundingBox();*/
}
void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	if (level == MARIO_LEVEL_BIG)
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
		/*if (state == GOOMBA_STATE_DIE)
			bottom = y + 18;*/
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_TAIL)
	{
		right = x + MARIO_TAIL_BBOX_WIDTH;
		bottom = y + MARIO_TAIL_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_FIRE)
	{
		right = x + MARIO_FIRE_BBOX_WIDTH;
		bottom = y + MARIO_FIRE_BBOX_HEIGHT;
	}
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		nx = 1;
		if (!SpeedInertia())
			vx = MARIO_WALKING_SPEED;
		break;
	case MARIO_STATE_WALKING_LEFT:
		nx = -1;
		if (!SpeedInertia())
			vx = -(MARIO_WALKING_SPEED);
		break;
	case MARIO_STATE_RUNNING_RIGHT:
		nx = 1;
		if (!SpeedInertia())
		{
			if (vx < MARIO_MAX_SPEED)
				vx += current_level_speed_up * SPEECH_ADDTION_PER_LEVEL;
		}
		break;
	case MARIO_STATE_RUNNING_LEFT:
		nx = -1;
		if (!SpeedInertia())
		{
			if (vx > -MARIO_MAX_SPEED)
				vx -= current_level_speed_up * SPEECH_ADDTION_PER_LEVEL;
		}
		break;
		//case MARIO_STATE_BRAKING_RIGHT:
		//	vx -= (MARIO_WALKING_GIATOC * 4) * dt;
		//	if (vx <= 0)
		//		vx = 0;
		//	break;
	case MARIO_STATE_JUMP:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		vy = -MARIO_JUMP_SPEED_Y;
		break;
	case MARIO_STATE_SITDOWN:
		vx = 0;
		break;
	case MARIO_STATE_IDLE:
		vx = 0;
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	case MARIO_STATE_SPEEDING_DOWN:
		SpeedDown();
		break;
	case MARIO_STATE_FLY:
		vy = -MARIO_FLYING_SPEED;
		break;
	case MARIO_STATE_FALL_DOWN:
		vy = MARIO_FLYING_SPEED;
		break;
	
	}
	// count down level

}



/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void CMario::SetLevel(int l)
{
	int oldlevel = this->level;
	this->level = l;
	if (level == MARIO_LEVEL_SMALL)
	{
		y += MARIO_DIFFERENCE_HEIGHT;
	}
	else if (oldlevel == MARIO_LEVEL_SMALL)
	{
		y -= MARIO_DIFFERENCE_HEIGHT;
	}
}
void CMario::SpeedDown()
{
	if (nx > 0)
	{
		vx -= MARIO_WALKING_SPEED / 40;
	}
	else if (nx < 0)
	{
		vx += MARIO_WALKING_SPEED / 40;
	}
}
bool CMario::SpeedInertia()
{
	if (vx*nx < 0) // ngược hướng
	{
		{
			if (nx > 0)
			{
				vx += MARIO_WALKING_SPEED / 10;
			}
			else if (nx < 0)
			{
				vx -= MARIO_WALKING_SPEED / 10;
			}
		}
		isBraking = true;
		return true;
	}
	else
	{
		isBraking = false;
		return false;
	}
	
}
void CMario::Fire()
{
	vector<LPGAMEOBJECT> cartridge_clip = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->Get_cartridge_clip();
	for (UINT i = 0; i < cartridge_clip.size(); i++)
	{
		CFireBullet *bullet = dynamic_cast<CFireBullet *>(cartridge_clip[i]);
		if (bullet->GetIsUsed() == false)
		{
			// get fired
			bullet->SetState(BULLET_STATE_FLYING);
			return;
		}
	}
}
void CMario::StartFlying()
{
	if (flying_start == 0 && isLanding ==false)
	{
		flying_start = GetTickCount();
		vy = -MARIO_FLYING_SPEED;
	}
	else
	{
		if (GetTickCount() - flying_start > MARIO_FLYING_TIME)
		{
			flying_start = 0;
			isLanding = true;
			canFly = false;
		}
	}
}