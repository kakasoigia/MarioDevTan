#include "FlowerBullet.h"

void CFlowerBullet::CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents)
{

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (!dynamic_cast<CMario *>(coObjects->at(i)))
		{
			continue;
		}

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);

}

void CFlowerBullet::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{

	CGameObject::Update(dt);



	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();



	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	//CalcPotentialCollisions(coObjects, coEvents);

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CFlower *>(obj))
		{
			CFlower *flower = dynamic_cast<CFlower *>(obj);

			if (flower->GetIsFiring() && !isUsed && flower->GetType() != FLOWER_GREEN)
			{
				if (!flower->GetIsFired())
				{
					y = flower->y + 5;
					if (mario->x <= flower->x)
					{
						x = flower->x - 1;
						if (flower->GetType() == FLOWER_RED)
						{
							if (mario->x >= FLOWER_BULLET_FIRST_X_LIMIT)
							{
								vx = -FLOWER_BULLET_FLYING_SPEED;
								vy = 0.05f;
							}
							else
							{
								vx = -(FLOWER_BULLET_FLYING_SPEED *1.1f);
								vy = 0.02f;
							}
						}
						else
						{
							if (mario->x >= FLOWER_BULLET_THIRD_X_LIMIT)
							{
								vx = -0.04f;
								vy = 0.04f;
							}
							else
							{
								vx = -(FLOWER_BULLET_FLYING_SPEED *1.1f);
								vy = 0.02f;
							}
						}
					}
					else
					{
						if (flower->GetType() == FLOWER_RED)
						{
							x = flower->x + FLOWER_RED_BBOX_WIDTH + 2;
							if (mario->x <= FLOWER_BULLET_SECOND_X_LIMIT)
							{
								vx = FLOWER_BULLET_FLYING_SPEED;
								vy = 0.05f;
							}
							else
							{
								vx = FLOWER_BULLET_FLYING_SPEED * 1.1f;
								vy = 0.02f;
							}
						}
						else
						{
							x = flower->x + FLOWER_GREEN_CAN_SHOOT_BBOX_WIDTH + 2;
							if (mario->x <= FLOWER_BULLET_FOURTH_X_LIMIT)
							{
								vx = 0.03f;
								vy = 0.04f;
							}
							else
							{
								vx = 0.04f * 1.3f;
								vy = 0.02f;
							}
						}
					}
					SetState(FLOWER_BULLET_STATE_FLYING);
					flower->SetIsFired(true);
				}
			}
		}
	}



	if (this->y >= 170)
	{
		isUsed = false;
		SetState(FLOWER_BULLET_STATE_HIDDEN);

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

		// block 

		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;


		// Collision logic with the others Koopas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}


void CFlowerBullet::Render()
{
	int ani = -1;

	if (isUsed)
	{
		ani = FLOWER_BULLET_ANI;
	}
	else return;

	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void CFlowerBullet::SetState(int state)
{
	CGameObject::SetState(state);
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (state)
	{
	case FLOWER_BULLET_STATE_FLYING:
		isUsed = true;
		break;
	case FLOWER_BULLET_STATE_HIDDEN:
		vx = 0;
		vy = 0;
		SetPosition(2000, 2000);
		break;
	}


}

void CFlowerBullet::GetBoundingBox(float &l, float &t, float &r, float &b)
{

	l = x;
	t = y;
	r = x + FLOWER_BULLET_BBOX_WIDTH;
	b = y + FLOWER_BULLET_BBOX_HEIGHT;
}
