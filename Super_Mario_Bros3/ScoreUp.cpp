#include "ScoreUp.h"



CScoreUp::CScoreUp()
{
	isUsed = false;
	SetState(SCORE_STATE_IDLE);
}




void CScoreUp::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = t = r = b = 0;
}

void CScoreUp::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	
			if (state == SCORE_STATE_UP)
			{
				if (GetTickCount() - timing_start >= 600)
				{
					isUsed = false;
					SetPosition(6000, 6000);
					SetState(SCORE_STATE_IDLE);
					timing_start = 0;
					value = 0;
				}
				else
				{
					x += dx;
					y += dy;
				}
			}
	
}

void CScoreUp::Render()
{
	int ani = -1;
	if (isUsed  && value != 0 && value != 10)
	{
		switch (value)
		{
		case SCORE_TYPE_100:
			ani = SCORE_100_ANI;
			break;
		case SCORE_TYPE_200:
			ani = SCORE_200_ANI;
			break;
		case SCORE_TYPE_400:
			ani = SCORE_400_ANI;
			break;
		case SCORE_TYPE_800:
			ani = SCORE_800_ANI;
			break;
		case SCORE_TYPE_1000:
			ani = SCORE_1000_ANI;
			break;
		case SCORE_TYPE_2000:
			ani = SCORE_2000_ANI;
			break;
		case SCORE_TYPE_4000:
			ani = SCORE_4000_ANI;
			break;
		case SCORE_TYPE_8000:
			ani = SCORE_8000_ANI;
			break;
		case SCORE_TYPE_1LIFE:
			ani = SCORE_1LIFE_ANI;
			break;
		}
	}
	else return;
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CScoreUp::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case  SCORE_STATE_IDLE:
		vx = vy = 0;
		break;
	case  SCORE_STATE_UP:
		vy = -0.08f;
		break;
	}
}