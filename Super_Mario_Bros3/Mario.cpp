#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"
#include "Rectangle.h"
#include "Goomba.h"
#include "Portal.h"
#include "Koopas.h"
#include "Coin.h"
#include "BreakableBrick.h"
#include "Bell.h"
#include "QuestionBrick.h"
#include "Leaf.h"
#include "MushRoom.h"
#include "ScoreUp.h"
#include "SpecialItem.h"
#include "HudPanels.h"
#include "PlayScence.h"
#include "Boomerang.h"
#include "BoomerangEnemy.h"
#include "FloatingWood.h"
CMario::CMario(float x, float y) : CGameObject()
{
	CGame *game = CGame::GetInstance();
	CoinCounter = game->GetCoinCounter();
	Score = game->GetScore();
	life_counter = game->GetLifeCounter();
	itemList = game->GetItemList();

	level = game->GetMarioLevel();
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	current_level_speed_up = 0;

}
void CMario::FilterCollision(vector<LPCOLLISIONEVENT> &coEvents, vector<LPCOLLISIONEVENT> &coEventsResult, float &min_tx, float &min_ty, float &nx, float &ny, float &rdx, float &rdy)
{

	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
		}

		if (c->t < min_ty  && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
		}
		if (dynamic_cast<CCoin *>(c->obj))
		{
			nx = 0;
			ny = 0;
		}
		if (dynamic_cast<CMushRoom *>(c->obj) || dynamic_cast<CLeaf *>(c->obj) || dynamic_cast<CFlowerBullet *>(c->obj) || dynamic_cast<CKoopas *>(c->obj) || dynamic_cast<CGoomba *>(c->obj) || dynamic_cast<CBoomerang *>(c->obj))
		{
			ny = -0.0001f;
		}
		if (dynamic_cast<CBreakableBrick *>(c->obj))
		{
			CBreakableBrick *breakable_brick = dynamic_cast<CBreakableBrick *> (c->obj);
			if (breakable_brick->GetState() == BREAKABLE_STATE_COIN)
			{
				if (isJumping)
				{
					nx = 0;
					ny = 0;
				}
			}
		}
		if (dynamic_cast<CFloatingWood *>(c->obj))
		{
			nx = -1.5f;

		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}
void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{


	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	if (state != MARIO_STATE_FLY && state != MARIO_STATE_FALL_DOWN && state != MARIO_STATE_PIPE_SLIDE_DOWN && state != MARIO_STATE_PIPE_SLIDE_UP && !isTransforming)
		vy += MARIO_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	//out of screen ->die
	if (y >= CGame::GetInstance()->GetScreenHeight() * 3 && state != MARIO_STATE_DIE && !isAtTunnel)
	{
		SetState(MARIO_STATE_DIE);
	}
	// turn off collision when die 
	if (state != MARIO_STATE_DIE && state != MARIO_STATE_PIPE_SLIDE_DOWN && state != MARIO_STATE_PIPE_SLIDE_UP)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	if (GetTickCount() - firing_start > MARIO_TIME_FIRING)
	{
		firing_start = 0;
		isFiring = false;
	}
	if (GetTickCount() - kicking_start > MARIO_KICK_TIME)
	{
		kicking_start = 0;
		isKicking = false;
	}
	if (GetTickCount() - turning_start > MARIO_TURNING_TIME)
	{
		turning_start = 0;
		isTurning = false;
	}
	if (start_time_die_back_to_worldmap != 0)
	{
		if (GetTickCount() - start_time_die_back_to_worldmap > MARIO_TIME_BACK_TO_WORLDMAP)
		{
			start_time_die_back_to_worldmap = 0;
			CGame::GetInstance()->SwitchScene(2);
		}
	}
	if (state == MARIO_STATE_PIPE_SLIDE_DOWN)
	{

		if (GetTickCount() - pipe_slide_down_start >= MARIO_TIME_SLIDE_UP_DOWN)
		{
			int id = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetId();
			if (id != 4)
			{
				this->SetPosition(POS_X_TUNNEL_MAP_1, POS_Y_TUNNEL_MAP_1);
				canPipeSlideDown = false;
				isAtTunnel = true;
			}
			else
			{
				this->SetPosition(POS_X_SCENE_END_MAP_4, POS_Y_SCENE_END_MAP_4);
				canPipeSlideDown = false;
				isAtTunnel = false;
			}

			SetState(MARIO_STATE_IDLE);
			pipe_slide_down_start = 0;
		}
	}

	if (state == MARIO_STATE_PIPE_SLIDE_UP)
	{

		if (GetTickCount() - pipe_slide_up_start >= MARIO_TIME_SLIDE_UP_DOWN)
		{
			isAtTunnel = false;
			if (!setPositionOutOfTunnel)
			{
				this->SetPosition(POS_X_OUT_OF_TUNNEL_MAP_1, POS_Y_OUT_OF_TUNNEL_MAP_1);
				setPositionOutOfTunnel = true;
			}
		}

		if (GetTickCount() - pipe_slide_up_start >= 6350)
		{
			canPipeSlideUp = false;
			SetState(MARIO_STATE_IDLE);
			pipe_slide_up_start = 0;
			setPositionOutOfTunnel = false;
		}
	}
	if (isTransforming)
	{
		vx = 0;
	}
	if (transforming_start != 0)
	{
		if (GetTickCount() - transforming_start >= MARIO_TIME_TRANSFORM)
		{

			isTransforming = false;
			transforming_start = 0;

			if (!transformUpLevel)
			{
				if (level == MARIO_LEVEL_TAIL)
				{
					SetLevel(MARIO_LEVEL_BIG);
				}
				else
				{
					SetLevel(MARIO_LEVEL_SMALL);
				}
			}
		}

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
			isHitted = false;
			canFly = true;
		}

		/*if (coEventsResult.size() == 0) isJumping = true;*/

		//
		// Collision logic with other objects

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{

			LPCOLLISIONEVENT e = coEventsResult[i];
			//if (e->ny < 0) isJumping = false; // chạm vật có thể nhảy tiếp
			/*if (!dynamic_cast<CPipe *>(e->obj))
				if (ny != 0) vy = 0;*/
			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						if (goomba->GetType() != GOOMBA_TYPE_RED_FLY) // not red flhy
						{

							goomba->SetState(GOOMBA_STATE_DIE);
							IncScore(100, goomba->x, goomba->y);
							vy = -MARIO_JUMP_DEFLECT_SPEED;
						}
						else// redfly
						{
							goomba->SetType(GOOMBA_TYPE_RED_WALK);
							goomba->SetState(GOOMBA_STATE_WALKING);
							IncScore(100, goomba->x, goomba->y);
							vy = -MARIO_JUMP_DEFLECT_SPEED;



						}
					}
				}
				else if (e->nx != 0 || e->ny > 0)
				{
					if (level == MARIO_LEVEL_TAIL && isTurning)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE_BY_KICK)
						{
							goomba->SetState(GOOMBA_STATE_DIE_BY_KICK);
							IncScore(100, goomba->x, goomba->y);
						}

					}
					else if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							SetTransformingDown();
						}
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
						if (koopas->GetType() == KOOPAS_TYPE_GREEN_FLY)
						{
							IncScore(100, koopas->x, koopas->y);
							koopas->SetType(koopas->GetType() - 1); //subtract 1 type from fly to walk
						}
						else if (koopas->GetType() == KOOPAS_TYPE_RED_FLY)
						{
							koopas->SetType(KOOPAS_TYPE_RED_WALK);
							koopas->SetState(KOOPAS_STATE_WALKING);
							vy = -1.5f * MARIO_JUMP_DEFLECT_SPEED;
							vx = 0.3f;


						}
						else
						{
							IncScore(100, koopas->x, koopas->y);
							koopas->SetState(KOOPAS_STATE_SHELL);
						}
						vy = -2 * MARIO_JUMP_DEFLECT_SPEED;
					}

					else if (koopas->GetState() == KOOPAS_STATE_SHELL)
					{
						koopas->SetState(KOOPAS_STATE_SPINNING);
						IncScore(100, koopas->x, koopas->y);
						vy = -2 * MARIO_JUMP_DEFLECT_SPEED;
					}
					else
					{
						if (e->nx != 0)
						{
							if (untouchable == 0)
							{
								if (koopas->GetState() != KOOPAS_STATE_DIE)
								{
									SetTransformingDown();
								}
							}
						}
					}
				}
				else if (e->nx != 0)
				{
					if (level == MARIO_LEVEL_TAIL && isTurning)
					{
						//deflect a bit

						/*koopas->vx += 0.3f * this->nx;*/
						if (koopas->GetState() != KOOPAS_STATE_SHELL)
							koopas->SetState(KOOPAS_STATE_SHELL);
						koopas->vy = -0.2f;
						koopas->vx = 0.1f * (-nx);

					}
					else if (koopas->GetState() == KOOPAS_STATE_SHELL)
					{
						//if (ny == 0) // kick Koo || hold Koo
						//{
						if (isHolding /*; state == MARIO_STATE_RUNNING_LEFT || state == MARIO_STATE_RUNNING_RIGHT*/)
						{

							koopas->SetIsHolding(true);
						}
						else
						{
							isHolding = false;
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
						/*}*/
					}
					else if (koopas->GetState() != KOOPAS_STATE_SHELL && isKicking == false && isTurning == false)
					{
						if (untouchable == 0)
						{
							if (koopas->GetState() != KOOPAS_STATE_DIE)
							{
								SetTransformingDown();
							}
						}
					}
					else
					{

						if (untouchable == 0)
						{
							if (koopas->GetState() != KOOPAS_STATE_DIE)
							{
								SetTransformingDown();
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
			else if (dynamic_cast<CCoin *>(e->obj)) // if e->obj is Coin
			{

				CCoin *coin = dynamic_cast<CCoin *>(e->obj);
				coin->SetIsAppear(false);

				IncScore(100, coin->x, coin->y);
				CoinCounterUp();

			}
			else if (dynamic_cast<CFlower *>(e->obj))
			{
				if (untouchable == 0)
				{
					SetTransformingDown();

				}
			}
			else if (dynamic_cast<CFlowerBullet *>(e->obj))
			{
				if (untouchable == 0)
				{
					SetTransformingDown();

				}
			}
			else if (dynamic_cast<CQuestionBrick *>(e->obj))
			{

				CQuestionBrick *question_brick = dynamic_cast<CQuestionBrick *>(e->obj);
				if (e->ny > 0)
				{

					if (question_brick->GetIsAlive() && !question_brick->GetIsAllowQuestionBrickSlide())
					{
						question_brick->SetState(QUESTION_BRICK_STATE_USED, coObjects);
						question_brick->SetIsUp(true);
						if (!question_brick->GetIsAllowQuestionBrickSlide())
							question_brick->SetIsAllowQuestionBrickSlide(true);
					}


				}
				if (e->nx != 0)
				{

					if (isTurning && (question_brick->y >= this->y + MARIO_TURNING_BONUS_HEIGHT))
					{

						if (question_brick->GetIsAlive() && !question_brick->GetIsAllowQuestionBrickSlide())
						{
							question_brick->SetState(QUESTION_BRICK_STATE_USED, coObjects);
							question_brick->SetIsUp(true);
							if (!question_brick->GetIsAllowQuestionBrickSlide())
								question_brick->SetIsAllowQuestionBrickSlide(true);
						}
					}

				}

			}
			else if (dynamic_cast<CBreakableBrick *>(e->obj))
			{

				CBreakableBrick *brick = dynamic_cast<CBreakableBrick *>(e->obj);
				if (brick->GetState() == BREAKABLE_STATE_COIN) // if COIN ..
				{
					brick->SetState(BREAKABLE_STATE_DISAPPEAR);
					CoinCounterUp();

				}
				else // another situattion
				{
					if (e->ny > 0)
					{
						if (brick->GetType() == BREAKABLE_BRICK_NORMAL)
						{
							if (this->level == MARIO_LEVEL_SMALL)
							{
								brick->SetIsBouncing(true);

							}
							else
							{
								brick->SetState(BREAKABLE_STATE_BROKEN);
								IncScore(10, brick->x, brick->y);
							}
						}
						else
						{
							brick->SetState(BREAKABLE_STATE_EMPTY_BOX);
							//find Bell available at same place
							for (UINT i = 0; i < coObjects->size(); i++)
							{
								LPGAMEOBJECT obj = coObjects->at(i);
								if (dynamic_cast<CBell *>(obj))
								{
									////check position
									if (brick->x == obj->x && brick->y == obj->y)
									{
										CBell *bell = dynamic_cast<CBell *>(obj);
										bell->SetState(BELL_STATE_SHOW);
										bell->y = brick->y - 16;
									}
								}
							}
						}
					}
					if (e->nx != 0)
					{

						if (isTurning && (brick->y >= this->y + MARIO_TURNING_BONUS_HEIGHT))
						{
							if (brick->GetType() == BREAKABLE_BRICK_NORMAL)
							{
								brick->SetState(BREAKABLE_STATE_BROKEN);
								IncScore(10, brick->x, brick->y);
							}
							else if (brick->GetState() == BREAKABLE_BRICK_BELL)
							{
								brick->SetState(BREAKABLE_STATE_EMPTY_BOX);
								//find Bell available at same place
								for (UINT i = 0; i < coObjects->size(); i++)
								{
									LPGAMEOBJECT obj = coObjects->at(i);
									if (dynamic_cast<CBell *>(obj))
									{
										////check position
										if (brick->x == obj->x && brick->y == obj->y)
										{
											CBell *bell = dynamic_cast<CBell *>(obj);
											bell->SetState(BELL_STATE_SHOW);
											bell->y = brick->y - 16;
										}
									}
								}
							}
						}

					}
				}

			}
			else if (dynamic_cast<CFloatingWood *>(e->obj))
			{

				CFloatingWood *floating_wood = dynamic_cast<CFloatingWood *>(e->obj);
				if (e->ny < 0)
				{

					if (floating_wood->GetState() == FLOATING_WOOD_STATE_NORMAL)
					{
						floating_wood->SetState(FLOATING_WOOD_STATE_DOWN);
					}
					/*if (level != MARIO_LEVEL_SMALL)
					{
						y = this->y - 27;
					}
					else
					{
						y = this->y - 15;
					}*/

				}

			}
			else if (dynamic_cast<CBell *>(e->obj))
			{
				CBell *bell = dynamic_cast<CBell *>(e->obj);
				if (e->ny < 0)
				{
					if (bell->GetState() != BELL_STATE_PRESSED)
						bell->SetActive(true);
				}
			}
			else if ((dynamic_cast<CLeaf *>(e->obj)))
			{
				CLeaf *leaf = dynamic_cast<CLeaf *>(e->obj);
				if (GetLevel() == MARIO_LEVEL_SMALL)
				{
					SetTransformingUp(1); // BIG
					leaf->SetIsAppear(false);

				}
				else if (GetLevel() == MARIO_LEVEL_BIG)
				{
					SetTransformingUp(2); // tail
					leaf->SetIsAppear(false);

				}
				else
				{
					//SetTransformingUp(2); // tail
					leaf->SetIsAppear(false);
				}
				IncScore(1000, leaf->x, leaf->y);
			}
			else if ((dynamic_cast<CMushRoom *>(e->obj)))
			{
				CMushRoom *mushroom = dynamic_cast<CMushRoom *>(e->obj);

				if (mushroom->GetType() == MUSHROOM_RED)
				{
					if (GetLevel() == MARIO_LEVEL_SMALL)
					{
						SetTransformingUp(1); // BIG
						mushroom->SetIsAppear(false);
						IncScore(1000, mushroom->x, mushroom->y);
					}
					else
					{
						mushroom->SetIsAppear(false);
						IncScore(1000, mushroom->x, mushroom->y);
						//Cong diem
					}

				}
				else // green mush
				{
					mushroom->SetIsAppear(false);
					AddLifeCounter();
					IncScore(SCORE_TYPE_1LIFE, mushroom->x, mushroom->y);
				}
			}
			else if (dynamic_cast<CSpecialItem *>(e->obj))
			{

				CSpecialItem *special_item = dynamic_cast<CSpecialItem *>(e->obj);
				int special_item_state = special_item->GetState();
				switch (special_item_state)
				{
				case SPECIAL_ITEM_STATE_FLOWER_IDLE:
					special_item->SetState(SPECIAL_ITEM_STATE_FLOWER_UP);
					AddItem(SPECIAL_ITEM_STATE_FLOWER_IDLE);
					break;
				case SPECIAL_ITEM_STATE_MUSHROOM_IDLE:
					special_item->SetState(SPECIAL_ITEM_STATE_MUSHROOM_UP);
					AddItem(SPECIAL_ITEM_STATE_MUSHROOM_IDLE);
					break;
				case SPECIAL_ITEM_STATE_STAR_IDLE:
					special_item->SetState(SPECIAL_ITEM_STATE_STAR_UP);
					AddItem(SPECIAL_ITEM_STATE_STAR_IDLE);
					break;
				}
				isAutoWalk = true;
				SetState(MARIO_STATE_WALKING_RIGHT);
				StartTimeBackToWorld();

			}
			else if (dynamic_cast<CPipe *>(e->obj))
			{
				CPipe* pipe = dynamic_cast<CPipe *>(e->obj);
				if (pipe->GetType() == PIPE_TYPE_DOWN)
				{
					if ((pipe->x + 4) <= this->x && this->x <= (pipe->x + 14))
					{
						this->canPipeSlideDown = true;
					}
					else
					{
						this->canPipeSlideDown = false;
					}
				}
				else if (pipe->GetType() == PIPE_TYPE_UP)
				{
					if ((pipe->x + 4) <= this->x && this->x <= (pipe->x + 14))
					{
						this->canPipeSlideUp = true;
					}
					else
					{
						this->canPipeSlideUp = false;
					}
				}
			}
			else if (dynamic_cast<CBoomerang *>(e->obj))
			{
				if (untouchable == 0)
				{
					if (level > MARIO_LEVEL_SMALL)
					{
						SetTransformingDown();
					}
					else
						SetState(MARIO_STATE_DIE);

				}
			}
			else if (dynamic_cast<CBoomerangEnemy *>(e->obj))
			{
				CBoomerangEnemy* boomerang_enemy = dynamic_cast<CBoomerangEnemy *>(e->obj);
				if (e->ny < 0)
				{
					if (boomerang_enemy->GetIsAlive())
					{
						boomerang_enemy->SetIsAlive(false);
						boomerang_enemy->SetState(BOOMERANG_ENEMY_STATE_DIE);
						boomerang_enemy->SetIsAllowToHaveBBox(false);
						vy = -1.5f * MARIO_JUMP_DEFLECT_SPEED;
					}

					IncScore(1000, boomerang_enemy->x, boomerang_enemy->y);
				}
				else if (level == MARIO_LEVEL_TAIL && isTurning)
				{
					boomerang_enemy->SetIsAlive(false);
					boomerang_enemy->SetState(BOOMERANG_ENEMY_STATE_DIE);
					boomerang_enemy->SetIsAllowToHaveBBox(false);
					boomerang_enemy->vy = -KOOPAS_SHELL_DEFLECT_SPEED;
					IncScore(100, boomerang_enemy->x, boomerang_enemy->y);
				}
				else
				{
					if (untouchable == 0)
					{
						if (level > MARIO_LEVEL_SMALL)
						{
							SetTransformingDown();
						}
						else
							SetState(MARIO_STATE_DIE);

					}
				}
			}

		

		// map 1-4



		/*DebugOut(L" update xong   \n");*/
	}

}


// clean up collision events
for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
// Update camera to follow mario
float cx = this->x, cy = this->y;
/*player->GetPosition(cx, cy);*/

CGame *game = CGame::GetInstance();
cx -= game->GetScreenWidth() / 2;
cy -= game->GetScreenHeight() / 2;

float camX = 0;
float camY = 0;
bool camcanmove = (CPlayScene*)CGame::GetInstance()->GetCurrentScene()->GetCamCanMove();
if (!camcanmove)
{
	if (x > (game->GetScreenWidth() / 2)) camX = cx;
	if (GetState() == MARIO_STATE_FLY /*|| GetState() == MARIO_STATE_FALL_DOWN*/ || GetIsLanding() == true
		|| y < -100 || GetState() == MARIO_STATE_PIPE_SLIDE_DOWN || GetState() == MARIO_STATE_PIPE_SLIDE_UP)
	{
		/*if (y <= (game->GetScreenHeight() / 2))*/ camY = cy + 20;
	}
	if (state != MARIO_STATE_DIE)
	{
		if (x > 2649) camX = 2484;
		CGame::GetInstance()->SetCamPos((int)camX, (int)camY - 70);
		/*if (isAutoWalk) CGame::GetInstance()->SetCamPos(2450, -50);*/
		if (isAtTunnel)
		{
			CGame::GetInstance()->SetCamPos(1300, 980);
		}
		if (isAutoWalk)
		{
			CGame::GetInstance()->SetCamPos(2500, -62);
		}
	}


	/*	else if (x >)
		{

		}*/
}
else
{
	if (x > (game->GetScreenWidth() / 2)) camX = cx;


	if (x < 1870) //
		camX_update += 0.035f * dt;
	else
	{
		camX_update = camX;
	}


	if (x < camX_update)
	{
		x = camX_update;
		isCamPushed = true;
	}
	else
	{
		isCamPushed = false;
	}
	CGame::GetInstance()->SetCamPos((int)camX_update, (int)210);
	if (x > (POS_X_EDGE_MAP_4 - game->GetScreenWidth()) && x < POS_X_EDGE_MAP_4 - 20)
	{
		CGame::GetInstance()->SetCamPos((int)POS_X_EDGE_MAP_4 - game->GetScreenWidth() - 7, (int)210);
	}
	else if (x > (POS_X_EDGE_MAP_4) && x < (POS_X_EDGE_MAP_4 + game->GetScreenWidth() / 2))
	{
		CGame::GetInstance()->SetCamPos((int)POS_X_EDGE_MAP_4 + 8, (int)210);
	}
	else if (x > 2408)
	{
		CGame::GetInstance()->SetCamPos((int)2243, (int)210);
	}

	/*if (isAtTunnel)
	{
		CGame::GetInstance()->SetCamPos(1300, 980);
	}*/
	if (isAutoWalk)
	{
		CGame::GetInstance()->SetCamPos(2243, 200);
	}

}


}

void CMario::Render()
{
	/*DebugOut(L" level la %d,", level);
	DebugOut(L" state %d,", state);*/
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
	else if (state == MARIO_STATE_PIPE_SLIDE_UP || state == MARIO_STATE_PIPE_SLIDE_DOWN)
	{
		if (level == MARIO_LEVEL_BIG)
		{
			ani = MARIO_ANI_BIG_PIPE;
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			ani = MARIO_ANI_SMALL_PIPE;
		}
		else if (level == MARIO_LEVEL_TAIL)
		{
			ani = MARIO_ANI_TAIL_PIPE;
		}
		else
		{
			ani = MARIO_ANI_FIRE_PIPE;
		}
	}
	else if (state == MARIO_STATE_FALL_DOWN)
	{

		if (level == MARIO_LEVEL_TAIL)
		{
			if (nx > 0) ani = MARIO_ANI_FALLING_RIGHT;
			else ani = MARIO_ANI_FALLING_LEFT;
		}
	}
	else if (isTurning)
	{
		if (nx < 0) ani = MARIO_ANI_TAIL_TURNING_LEFT;
		else ani = MARIO_ANI_TAIL_TURNING_RIGHT;
	}

	else if (isHoldAni)
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
	else if (isJumping == true) // đang trên kh ?
	{
		if (current_level_speed_up >= 7)
		{
			isJumpingMaxStack = true;

		}
		else
		{
			isJumpingMaxStack = false;
		}

		if (isJumpingMaxStack == false)
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
		else // max stack 
		{
			if (level == MARIO_LEVEL_BIG)
			{
				if (nx > 0)
				{
					ani = MARIO_ANI_BIG_JUMP_MAX_POWER_RIGHT;
				}
				else
				{
					ani = MARIO_ANI_BIG_JUMP_MAX_POWER_LEFT;
				}
			}
			else if (level == MARIO_LEVEL_SMALL)
			{
				if (nx > 0)
				{
					ani = MARIO_ANI_SMALL_JUMP_MAX_POWER_RIGHT;
				}
				else
				{
					ani = MARIO_ANI_SMALL_JUMP_MAX_POWER_LEFT;
				}
			}
			else if (level == MARIO_LEVEL_TAIL)
			{
				if (nx > 0)
				{
					ani = MARIO_ANI_TAIL_JUMP_MAX_POWER_RIGHT;
				}
				else
				{
					ani = MARIO_ANI_TAIL_JUMP_MAX_POWER_LEFT;
				}
			}
			else if (level == MARIO_LEVEL_FIRE)
			{
				if (nx > 0)
				{
					ani = MARIO_ANI_FIRE_JUMP_MAX_POWER_RIGHT;
				}
				else
				{
					ani = MARIO_ANI_FIRE_JUMP_MAX_POWER_LEFT;
				}
			}
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
	else if (isFiring)
	{
		if (nx < 0) ani = MARIO_ANI_FIRE_FIRING_BULLET_LEFT;
		else ani = MARIO_ANI_FIRE_FIRING_BULLET_RIGHT;
	}
	else if (state == MARIO_STATE_IDLE)
	{
		if (level == MARIO_LEVEL_BIG)
		{
			if (nx > 0) ani = MARIO_ANI_BIG_IDLE_RIGHT;
			else ani = MARIO_ANI_BIG_IDLE_LEFT;
			if (nx > 0)
			{
				if (isCamPushed)
				{
					ani = MARIO_ANI_BIG_WALKING_RIGHT;
				}
				else
					ani = MARIO_ANI_BIG_IDLE_RIGHT;
			}
			else
			{
				if (isCamPushed)
				{
					ani = MARIO_ANI_BIG_WALKING_LEFT;
				}
				else
					ani = MARIO_ANI_BIG_IDLE_LEFT;
			}
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
			else ani = MARIO_ANI_SMALL_IDLE_LEFT;
			if (nx > 0)
			{
				if (isCamPushed)
				{
					ani = MARIO_ANI_SMALL_WALKING_RIGHT;
				}
				else
					ani = MARIO_ANI_SMALL_IDLE_RIGHT;
			}
			else
			{
				if (isCamPushed)
				{
					ani = MARIO_ANI_SMALL_WALKING_LEFT;
				}
				else
					ani = MARIO_ANI_SMALL_IDLE_LEFT;
			}
		}
		else if (level == MARIO_LEVEL_TAIL)
		{
			if (nx > 0) ani = MARIO_ANI_TAIL_IDLE_RIGHT;
			else ani = MARIO_ANI_TAIL_IDLE_LEFT;
			if (nx > 0)
			{
				if (isCamPushed)
				{
					ani = MARIO_ANI_TAIL_WALKING_RIGHT;
				}
				else
					ani = MARIO_ANI_TAIL_IDLE_RIGHT;
			}
			else
			{
				if (isCamPushed)
				{
					ani = MARIO_ANI_TAIL_WALKING_LEFT;
				}
				else
					ani = MARIO_ANI_TAIL_IDLE_LEFT;
			}
		}
		else if (level == MARIO_LEVEL_FIRE)
		{
			if (nx > 0) ani = MARIO_ANI_FIRE_IDLE_RIGHT;
			else ani = MARIO_ANI_FIRE_IDLE_LEFT;
			if (nx > 0)
			{
				if (isCamPushed)
				{
					ani = MARIO_ANI_FIRE_WALKING_RIGHT;
				}
				else
					ani = MARIO_ANI_FIRE_IDLE_RIGHT;
			}
			else
			{
				if (isCamPushed)
				{
					ani = MARIO_ANI_FIRE_WALKING_LEFT;
				}
				else
					ani = MARIO_ANI_FIRE_IDLE_LEFT;
			}
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
		if ((nx < 0 && vx > -MARIO_MAX_SPEED) || (nx > 0 && vx < MARIO_MAX_SPEED)) // normal run
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
		else // max speed mode
		{
			if (level == MARIO_LEVEL_BIG)
			{
				ani = MARIO_ANI_BIG_MAX_SPEED_RIGHT;

			}
			else if (level == MARIO_LEVEL_SMALL)
			{
				ani = MARIO_ANI_SMALL_MAX_SPEED_RIGHT;

			}
			else if (level == MARIO_LEVEL_TAIL)
			{
				ani = MARIO_ANI_TAIL_MAX_SPEED_RIGHT;

			}
			else if (level == MARIO_LEVEL_FIRE)
			{
				ani = MARIO_ANI_FIRE_MAX_SPEED_RIGHT;

			}
		}

	}
	else if (state == MARIO_STATE_RUNNING_LEFT)
	{
		if ((nx<0 && vx > -MARIO_MAX_SPEED) || (nx > 0 && vx < MARIO_MAX_SPEED)) // normal run
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
		else
		{
			if (level == MARIO_LEVEL_BIG)
			{
				ani = MARIO_ANI_BIG_MAX_SPEED_LEFT;

			}
			else if (level == MARIO_LEVEL_SMALL)
			{
				ani = MARIO_ANI_SMALL_MAX_SPEED_LEFT;

			}
			else if (level == MARIO_LEVEL_TAIL)
			{
				ani = MARIO_ANI_TAIL_MAX_SPEED_LEFT;

			}
			else if (level == MARIO_LEVEL_FIRE)
			{
				ani = MARIO_ANI_FIRE_MAX_SPEED_LEFT;

			}
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

	if (isTransforming)
	{

		if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_SMALL)
		{

			if (nx > 0) ani = MARIO_TRANSFORM_RIGHT;
			else ani = MARIO_TRANSFORM_LEFT;


		}
		else if (level == MARIO_LEVEL_TAIL || level == MARIO_LEVEL_FIRE)
		{

			if (nx > 0)	ani = MARIO_SMOKE_TRANSFORM_RIGHT;
			else ani = MARIO_SMOKE_TRANSFORM_LEFT;

		}

	}



	int alpha = 255;
	if (untouchable) alpha = 128;
	/*DebugOut(L" ani %d,\n", ani);*/
	animation_set->at(ani)->Render(x, y, alpha);


	/*RenderBoundingBox();*/
}
void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == MARIO_STATE_DIE) left = top = right = bottom = 0;
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
		y -= 3;
		vx = 0;
		vy = -MARIO_DIE_DEFLECT_SPEED;
		if (life_counter >= 1)
			life_counter--;
		StartTimeBackToWorld();
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
	case MARIO_STATE_PIPE_SLIDE_DOWN:
		vy = 0.01f;
		break;
	case MARIO_STATE_PIPE_SLIDE_UP:
		vy = -0.01f;
		vx = 0;
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
void CMario::SetOnSpecialPosition(int place)
{
	switch (place)
	{
	case 1: // place  red goomba
		SetPosition(392, 117);
		SetSpeed(0, 0);
		break;
	case 2: // place 3 koopas
		SetPosition(630, 80);
		SetSpeed(0, 0);
		break;
	case 3: //place breakable brick
		SetPosition(2200, 80);
		SetSpeed(0, 0);
		break;
	case 4: //place breakable brick
		SetPosition(2256, -190);
		SetSpeed(0, 0);
		break;
		// map 1-4
	case 5: //place breakable brick
		SetPosition(645, 300);
		camX_update = 480;
		SetSpeed(0, 0);
		break;
	case 6: //place có nấm LV
		SetPosition(1000, 368);
		camX_update = 835;
		SetSpeed(0, 0);
		break;
	case 8: // tiền nhảy
		SetPosition(1536, 300);
		camX_update = 1371
			;
		SetSpeed(0, 0);
		break;
	case 7: // nấm xanh
		SetPosition(1183, 200);
		camX_update = 1018
			;
		SetSpeed(0, 0);
		break;
	case 9: // ống cống
		SetPosition(2220, 332);
		camX_update = 1915
			;
		SetSpeed(0, 0);
		break;
	case 10: // đầu ống
		SetPosition(1936, 300);
		camX_update = POS_X_EDGE_MAP_4
			;
		SetSpeed(0, 0);
		break;
	default:
		break;
	}
	SetState(MARIO_STATE_IDLE);
	isAtTunnel = false;




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
	if (isAutoWalk) return false;
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
	firing_start = GetTickCount();
	isFiring = true;
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
	if (flying_start == 0 && isLanding == false)
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
			current_level_speed_up = 0;
		}
	}
}
void CMario::IncScore(int score, float pos_x, float pos_y)
{

	if (score != SCORE_TYPE_1LIFE)
		Score += score;

	if (score == 10) return;
	vector<LPGAMEOBJECT> objects = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->Get_score_list();
	for (unsigned i = 0; i < objects.size(); i++) // find score free
	{
		CScoreUp *scoreUp = dynamic_cast<CScoreUp *>(objects[i]);
		if (!scoreUp->GetIsUsed())
		{
			scoreUp->SetIsUsed(true);
			scoreUp->SetPosition(pos_x, pos_y - 10);
			scoreUp->SetValue(score);

			scoreUp->SetState(SCORE_STATE_UP);
			scoreUp->StartTiming();

			return;
		}
	}

}
void CMario::SetTransformingDown()
{


	if (level > MARIO_LEVEL_SMALL)
	{
		isTransforming = true;
		transformUpLevel = false;
		StartTransforming();
		isFiring = false;
		StartUntouchable();
	}
	else
		SetState(MARIO_STATE_DIE);
}
void CMario::SetTransformingUp(int type)
{
	isTransforming = true;
	StartTransforming();
	transformUpLevel = true;
	isFiring = false;
	if (type == 1) // big
		SetLevel(MARIO_LEVEL_BIG);
	else if (type == 2) // tail
		SetLevel(MARIO_LEVEL_TAIL);
}



