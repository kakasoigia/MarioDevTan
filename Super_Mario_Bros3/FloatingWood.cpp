#include "FloatingWood.h"

CFloatingWood::CFloatingWood(int moving_horizontal_rectangle_id)
{
	this->id = moving_horizontal_rectangle_id;
	SetState(FLOATING_WOOD_STATE_NORMAL);
}

void CFloatingWood::CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (!dynamic_cast<CMario *>(coObjects->at(i)))
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
void CFloatingWood::GetBoundingBox(float &l, float &t, float &r, float &b)
{

	l = x;
	t = y;
	r = x + FLOATING_WOOD_BBOX_WIDTH;
	b = y + FLOATING_WOOD_BBOX_HEIGHT;


}

void CFloatingWood::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	/*if (mario->GetIsOnMovingHorizontalRectangle())
	{
		if (this->id == mario->GetMarioMovingHorizotalRecID())
		{
			if (mario->GetLevel() != MARIO_LEVEL_SMALL)
			{
				mario->y = this->y - 27;
			}
			else
			{
				mario->y = this->y - 15;
			}
		}
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

		// block 
		
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		/*y += min_ty * dy +  ny * 0.4f;*/

		//if (nx != 0) vx = 0;
		//if (ny != 0) vy = 0;

		// Collision logic with the others Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
		}
	}


	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	
}

void CFloatingWood::Render()
{
	animation_set->at(0)->Render(x, y);

	RenderBoundingBox();
}

void CFloatingWood::SetState(int state)
{
	CGameObject::SetState(state);
	//CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (state)
	{
	case  FLOATING_WOOD_STATE_NORMAL:
		vx = FLOATING_WOOD_STATE_NORMAL_SPEED;
		vy = 0;
		break;
	case   FLOATING_WOOD_STATE_DOWN:
		vy = FLOATING_WOOD_STATE_DOWN_SPEED;
		vx = 0;
		break;
	}
}

