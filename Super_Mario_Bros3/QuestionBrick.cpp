#include "QuestionBrick.h"

CQuestionBrick::CQuestionBrick(int ctype)
{
	type = ctype;
}

void CQuestionBrick::CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
		{
			coEvents.push_back(e);
		}
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}
void CQuestionBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + QUESTION_BRICK_BBOX_WIDTH;
	b = y + QUESTION_BRICK_BBOX_HEIGHT;
}

void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	//

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	/*CalcPotentialCollisions(coObjects, coEvents);*/
	// up a bit when being hit
	if (!isAlive)
	{
		if (isUp)
		{
			if (time_Y_Up > 4)
			{
				time_Y_Up = 0;
				isUp = false;
			}
			else
			{
				y -= 2;
				time_Y_Up++;
				//DebugOut(L"Nhun len \n");
			}
		}
		else
		{
			if (time_Y_Up > 4)
			{
				
			}
			else
			{
				y += 2;
				time_Y_Up++;
				//DebugOut(L"Nhun xuong \n");
			}
		}
	}


	//// No collision occured, proceed normally
	//if (coEvents.size() == 0)
	//{
	//	y += dy;
	//}
	//else
	//{

	//	float min_tx, min_ty, nx = 0, ny;
	//	float rdx = 0;
	//	float rdy = 0;

	//	// TODO: This is a very ugly designed function!!!!
	//	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

	//	// block 
	//	//x += min_tx * dx + nx * 0.5f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
	//	//y += min_ty * dy + ny * 0.5f;

	//	if (nx != 0) vx = 0;
	//	if (ny != 0) vy = 0;

	//	// Collision logic with the others Goombas
	//	for (UINT i = 0; i < coEventsResult.size(); i++)
	//	{
	//		LPCOLLISIONEVENT e = coEventsResult[i];


	//	}
	//}




	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CQuestionBrick::Render()
{
	int ani = -1;

	if (isAlive)
	{
		if (type == QUESTION_BRICK_JUST_HAVE_MUSHROOM)
			ani = QUESTION_BRICK_ANI_NEW_TYPE;
		else
			ani = QUESTION_BRICK_ANI_ALIVE;
	}
	else
		ani = QUESTION_BRICK_ANI_DEAD;
	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CQuestionBrick::SetState(int state, vector<LPGAMEOBJECT> *coObjects)
{
	if (state == QUESTION_BRICK_STATE_USED)
	{
		isAlive = false;
		CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		// if just have mushroom 
		if (type == QUESTION_BRICK_JUST_HAVE_MUSHROOM) // call green mushroom.
		{
			for (UINT i = 0; i < coObjects->size(); i++)
			{
				LPGAMEOBJECT obj = coObjects->at(i);
				if (dynamic_cast<CMushRoom *>(obj))
				{
					CMushRoom *mushroom = dynamic_cast<CMushRoom *>(obj);
					if (!mushroom->GetIsAppear() && mushroom->GetType() == MUSHROOM_GREEN && this->x == mushroom->x && this->y == mushroom->y) // unUsed and is Green and same position
					{
						mushroom->SetState(MUSHROOM_STATE_UP);
						return;
					}
				}
			}
		}
		else if (type == QUESTION_BRICK_HAVE_LEAF && mario->GetLevel() == MARIO_LEVEL_SMALL) // call mushroom red
		{
			for (UINT i = 0; i < coObjects->size(); i++)
			{
				LPGAMEOBJECT obj = coObjects->at(i);
				if (dynamic_cast<CMushRoom *>(obj))
				{
					CMushRoom *mushroom = dynamic_cast<CMushRoom *>(obj);
					if (!mushroom->GetIsAppear() && mushroom->GetType() == MUSHROOM_RED && this->x == mushroom->x && this->y == mushroom->y) // unUsed and is Green
					{
						mushroom->SetState(MUSHROOM_STATE_UP);
						return;
					}
				}
			}
		}
		else if(type == QUESTION_BRICK_HAVE_LEAF && mario->GetLevel() != MARIO_LEVEL_SMALL)  //call leaf
		{
			for (UINT i = 0; i < coObjects->size(); i++)
			{
				LPGAMEOBJECT obj = coObjects->at(i);
				if (dynamic_cast<CLeaf *>(obj))
				{
					CLeaf *leaf = dynamic_cast<CLeaf *>(obj);
					if (!leaf->GetIsAppear()  && this->x == leaf->x && this->y == leaf->y) // unUsed and is Green
					{
						leaf->SetState(LEAF_STATE_UP);
						return;
					}
				}
			}
		}
		else if (type == QUESTION_BRICK_NORMAL) // toss coin
		{

			for (UINT i = 0; i < coObjects->size(); i++)
			{
				LPGAMEOBJECT obj = coObjects->at(i);
				if (dynamic_cast<CCoin *>(obj))
				{
					CCoin *coin = dynamic_cast<CCoin *>(obj);
					if (!coin->GetIsAppear() && this->x == coin->x && this->y == coin->y) // unUsed and is Green
					{
						coin->SetState(COIN_STATE_UP);
						CMario* player = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
						player->CoinCounterUp();
						return;
					}
				}
			}
		}
		
	}
}
