#include "Flower.h"
#include "Pipe.h"
#include"Brick.h"
#include"Mario.h"
#include"PlayScence.h"
#include "Utils.h"
CFlower::CFlower(int ctype)
{
	type = ctype;
	vx = 0;
}




void CFlower::CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (dynamic_cast<CPipe*>(coObjects->at(i)))
		{
			continue;
		}
		if (e->t > 0 && e->t <= 1.0f)
		{
			coEvents.push_back(e);
		}
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}
void CFlower::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	switch (type)
	{
	case FLOWER_RED:
		l = x;
		t = y;
		r = x + FLOWER_RED_BBOX_WIDTH;
		b = y + FLOWER_RED_BBOX_HEIGHT;
		break;
	case FLOWER_GREEN:
		l = x;
		t = y;
		r = x + FLOWER_GREEN_BBOX_WIDTH;
		b = y + FLOWER_GREEN_BBOX_HEIGHT;
		break;
	case FLOWER_GREEN_CAN_SHOOT:
		l = x;
		t = y;
		r = x + FLOWER_GREEN_CAN_SHOOT_BBOX_WIDTH;
		b = y + FLOWER_GREEN_CAN_SHOOT_BBOX_HEIGHT;
		break;
	}

}

void CFlower::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	//

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	switch (type)
	{
	case FLOWER_RED:
		if (isUp)
		{
			if (time_showing == 0)
				StartShowing();
			if (GetTickCount() - time_showing <= RED_TIME_SHOWING_LIMIT)
			{
				vy = -0.02f;
				if (this->y <= FLOWER_RED_TOP_LIMIT)
				{
					vy = 0;
					isFiring = true;
					Shoot(coObjects);

				}
			}
			else
			{
				isUp = false;
				isFiring = false;
				isFired = false; //can fire again  
				time_showing = 0;

			}

		}
		else
		{

			if (time_showing == 0)
				StartShowing();
			if (GetTickCount() - time_showing <= RED_TIME_SHOWING_LIMIT)
			{
				vy = 0.02f;
				if (this->y >= FLOWER_RED_BOT_LIMIT)
				{
					vy = 0;
				}
			}
			else
			{
				isUp = true;
				time_showing = 0;
			}
		}
		break;
	case FLOWER_GREEN:
		if (isUp)
		{
			if (time_showing == 0)
				StartShowing();
			if (GetTickCount() - time_showing <= GREEN_TIME_SHOWING_LIMIT)
			{
				vy = -0.02f;
				if (this->y <= FLOWER_GREEN_TOP_LIMIT)
				{
					vy = 0;

				}
			}
			else
			{

				isUp = false;
				time_showing = 0;
			}

		}
		else
		{
			if (time_showing == 0)
				StartShowing();
			if (GetTickCount() - time_showing <= GREEN_TIME_SHOWING_LIMIT)
			{
				vy = 0.02f;
				if (this->y >= FLOWER_GREEN_BOT_LIMIT)
				{
					vy = 0;
				}
			}
			else
			{
				isUp = true;
				time_showing = 0;
			}
		}
		break;
	case FLOWER_GREEN_CAN_SHOOT:
		if (isUp)
		{
			if (time_showing == 0)
				StartShowing();
			else if (GetTickCount() - time_showing <= GREEN_CAN_SHOOT_TIME_SHOWING_LIMIT)
			{
				vy = -0.02f;
				if (this->y <= FLOWER_GREEN_CAN_SHOOT_TOP_LIMIT)
				{
					vy = 0;
					isFiring = true;
					DebugOut(L"[INFO] bắn \n");
					Shoot(coObjects);
				}
			}
			else
			{
				isUp = false;
				isFiring = false;
				isFired = false; // can fire again 
				time_showing = 0;
			}

		}
		else
		{
			if (time_showing == 0)
				StartShowing();
			else if (GetTickCount() - time_showing <= GREEN_CAN_SHOOT_TIME_SHOWING_LIMIT)
			{
				vy = 0.02f;
				if (this->y >= FLOWER_GREEN_CAN_SHOOT_BOT_LIMIT)
				{
					vy = 0;
				}
			}
			else
			{
				isUp = true;
				time_showing = 0;
			}
		}
		break;

	}
	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		y += dy;
	}
	else
	{

		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block 
		//x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		//y += min_ty * dy + ny * 0.4f;


		// Collision logic with the others Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];


		}




	}




	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CFlower::Render()
{
	int ani = -1;
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (type)
	{
	case FLOWER_RED:
		if (mario->x <= this->x)
		{
			if (mario->y >= this->y)
			{
				if (vy == 0)
				{
					ani = FLOWER_RED_ANI_LEFT_IDLE;
				}
				else
				{
					ani = FLOWER_RED_ANI_LEFT;
				}

			}
			else
			{
				if (vy == 0)
				{
					ani = FLOWER_RED_ANI_LEFT_IDLE_UP;
				}
				else
				{
					ani = FLOWER_RED_ANI_LEFT_UP;
				}
			}

		}
		else
		{
			if (mario->y >= this->y)
			{
				if (vy == 0)
				{
					ani = FLOWER_RED_ANI_RIGHT_IDLE;
				}
				else
				{
					ani = FLOWER_RED_ANI_RIGHT;
				}

			}
			else
			{
				if (vy == 0)
				{
					ani = FLOWER_RED_ANI_RIGHT_IDLE_UP;
				}
				else
				{
					ani = FLOWER_RED_ANI_RIGHT_UP;
				}
			}
		}
		break;

	case FLOWER_GREEN:
		ani = FLOWER_GREEN_ANI;
		break;
	case FLOWER_GREEN_CAN_SHOOT:

		if (mario->x <= this->x)
		{

			if (isShootingUp == 1)
			{
				if (vy == 0)
				{
					ani = FLOWER_GREEN_CAN_SHOOT_ANI_LEFT_IDLE;
				}
				else
				{
					ani = FLOWER_GREEN_CAN_SHOOT_ANI_LEFT;
				}
			}
			else
			{
				if (vy == 0)
				{
					ani = FLOWER_GREEN_CAN_SHOOT_ANI_LEFT_IDLE_UP;
				}
				else
				{
					ani = FLOWER_GREEN_CAN_SHOOT_ANI_LEFT_UP;
				}
			}

		}
		else
		{
			if (isShootingUp == 1)
			{
				if (vy == 0)
				{
					ani = FLOWER_GREEN_CAN_SHOOT_ANI_RIGHT_IDLE;
				}
				else
				{
					ani = FLOWER_GREEN_CAN_SHOOT_ANI_RIGHT;
				}

			}
			else
			{
				if (vy == 0)
				{
					ani = FLOWER_GREEN_CAN_SHOOT_ANI_RIGHT_IDLE_UP;
				}
				else
				{
					ani = FLOWER_GREEN_CAN_SHOOT_ANI_RIGHT_UP;
				}
			}
		}
		break;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CFlower::SetState(int state)
{
	/*CGameObject::SetState(state);*/
}
void CFlower::Shoot(vector<LPGAMEOBJECT> *coObjects)
{

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (abs(mario->x - this->x) >= 160 || isFired == true) return;
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CFlowerBullet *>(obj))
		{
			CFlowerBullet *bullet = dynamic_cast<CFlowerBullet *>(obj);
			if (bullet->GetIsUsed()) continue;// isUsed ..next
			if (this->isFiring && this->type != FLOWER_GREEN)
			{

				bullet->y = this->y + 5;
				//calc whether shoot up or down
				if (mario->y <= this->y)
				{
					this->SetIsShootingUp(-1);
				}
				else
				{
					this->SetIsShootingUp(1);
				}
				// calc bullet X 
				if (mario->x <= this->x)
				{
					bullet->x = this->x - 1;
					this->nx = -1;
				}
				else
				{
					bullet->x = this->x + FLOWER_RED_BBOX_WIDTH + 2;
					this->nx = 1;
				}
				//calc bullet direction
				if (abs(mario->x - this->x) <= FLOWER_BULLET_X_LIMIT)
				{
					bullet->vx = FLOWER_BULLET_FLYING_SPEED * this->nx;
					bullet->vy = 0.05f * this->isShootingUp;
				}
				else
				{
					bullet->vx = FLOWER_BULLET_FLYING_SPEED * this->nx;
					bullet->vy = 0.02f * this->isShootingUp;
				}
				bullet->SetState(FLOWER_BULLET_STATE_FLYING);
				isFiring = false;
				isFired = true; // have fired already..not fire again zzz
				return;
			}
			
		}
	}
}
