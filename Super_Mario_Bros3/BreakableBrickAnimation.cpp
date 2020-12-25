#include "BreakableBrickAnimation.h"




CBreakableBrickAnimation::CBreakableBrickAnimation(int ctype)
{
	type = ctype;
	SetState(BREAKABLE_BRICK_ANIMATION_STATE_IDLE);
}




void CBreakableBrickAnimation::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = t = r = b = 0;
}

void CBreakableBrickAnimation::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{

	CGameObject::Update(dt);


	x += dx;
	y += dy;

			if (state == BREAKABLE_BRICK_ANIMATION_STATE_MOVE)
			{
				if (GetTickCount() - timing_start >= 400)
				{
					vy += BREAKABLE_BRICK_ANIMATION_GRAVITY * dt;
				}
				if (GetTickCount() - timing_start >= 1000)
				{
					isUsed = false;
					SetPosition(12000, 12000);
					timing_start = 0;
				}
			}
	



}

void CBreakableBrickAnimation::Render()
{
	if (isUsed)
	{
		animation_set->at(0)->Render(x, y);
	}
	else return;


	//RenderBoundingBox();
}

void CBreakableBrickAnimation::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case  BREAKABLE_BRICK_ANIMATION_STATE_IDLE:
		vx = vy = 0;
		break;
	case  BREAKABLE_BRICK_ANIMATION_STATE_MOVE:
		switch (type)
		{
		case BREAKABLE_BRICK_ANIMATION_TYPE_LEFT_TOP:
			vx = -0.1f;
			vy = -0.2f;
			break;
		case BREAKABLE_BRICK_ANIMATION_TYPE_LEFT_BOTTOM:
			vx = -0.1f;
			vy = -0.1f;
			break;
		case BREAKABLE_BRICK_ANIMATION_TYPE_RIGHT_TOP:
			vx = 0.1f;
			vy = -0.2f;
			break;
		case BREAKABLE_BRICK_ANIMATION_TYPE_RIGHT_BOTTOM:
			vx = 0.1f;
			vy = -0.1f;
			break;
		}
		break;
	}
}
