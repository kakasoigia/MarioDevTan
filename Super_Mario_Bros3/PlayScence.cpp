﻿#include "PlayScence.h"
using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
	if (id == ID_MAP_4)
	{
		CamCanMove = true;
		/*CGame::GetInstance()->SetCamPos(0, 220);*/
	}
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/


void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (unsigned int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

	for (unsigned int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);

}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 7) return; // skip invalid lines

	int idTileSet = atoi(tokens[0].c_str());
	int totalRowsTileSet = atoi(tokens[1].c_str());
	int totalColumnsTileSet = atoi(tokens[2].c_str());
	int totalRowsMap = atoi(tokens[3].c_str());
	int totalColumnsMap = atoi(tokens[4].c_str());
	int totalTiles = atoi(tokens[5].c_str());
	wstring file_path = ToWSTR(tokens[6]);

	map = new Map(idTileSet, totalRowsTileSet, totalColumnsTileSet, totalRowsMap, totalColumnsMap, totalTiles);
	map->LoadMap(file_path.c_str());
	map->ExtractTileFromTileSet();
}

void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x, y);
		player = (CMario*)obj;

		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(1); break;
	case OBJECT_TYPE_GOOMBA_RED_FLY: obj = new CGoomba(2); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_KOOPAS_GREEN_WALK: obj = new CKoopas(1); break;
	case OBJECT_TYPE_KOOPAS_GREEN_FLY: obj = new CKoopas(2); break;
	case OBJECT_TYPE_KOOPAS_RED_WALK: obj = new CKoopas(3); break;
	case OBJECT_TYPE_KOOPAS_RED_FLY: obj = new CKoopas(4); break;
	case OBJECT_TYPE_RECTANGLE: obj = new CRectangle(); break;
	case OBJECT_TYPE_NO_COLLISION_OBJECTS:obj = new CNoCollisionObjects(3, 1); break;
	case OBJECT_TYPE_COIN_NORMAL:obj = new CCoin(COIN_NORMAL); break;
	case OBJECT_TYPE_COIN_CAN_TOSS:obj = new CCoin(COIN_CAN_TOSS); break;
	case OBJECT_TYPE_PIPE_NORMAL:obj = new CPipe(PIPE_TYPE_NORMAL); break;
	case OBJECT_TYPE_PIPE_CAN_SLIDE_DOWN:obj = new CPipe(PIPE_TYPE_DOWN); break;
	case OBJECT_TYPE_PIPE_CAN_SLIDE_UP:obj = new CPipe(PIPE_TYPE_UP); break;
	case OBJECT_TYPE_FIRE_BULLET:obj = new CFireBullet(); break;
	case OBJECT_TYPE_FLOWER_RED:	  obj = new CFlower(100); break;
	case OBJECT_TYPE_FLOWER_GREEN:	  obj = new CFlower(200); break;
	case OBJECT_TYPE_FLOWER_GREEN_CAN_SHOOT:   obj = new CFlower(300); break;
	case OBJECT_TYPE_FLOWER_BULLET:	   obj = new CFlowerBullet(); break;
	case OBJECT_TYPE_QUESTION_BRICK_NORMAL: obj = new CQuestionBrick(666); break;
	case OBJECT_TYPE_QUESTION_BRICK_HAVE_LEAF: obj = new CQuestionBrick(777); break;
	case OBJECT_TYPE_QUESTION_BRICK_JUST_HAVE_MUSHROOM: obj = new CQuestionBrick(888); break;
	case OBJECT_TYPE_LEAF:	           obj = new CLeaf(); break;
	case OBJECT_TYPE_MUSHROOM_RED:	   obj = new CMushRoom(567); break;
	case OBJECT_TYPE_MUSHROOM_GREEN:   obj = new CMushRoom(678); break;
	case OBJECT_TYPE_BREAKABLE_BRICK_NORMAL: obj = new CBreakableBrick(BREAKABLE_BRICK_NORMAL); break;
	case OBJECT_TYPE_BREAKABLE_BRICK_BELL: obj = new CBreakableBrick(BREAKABLE_BRICK_BELL); break;
	case OBJECT_TYPE_BELL: obj = new CBell(); break;
	case OBJECT_TYPE_HIT_EFFECT_TURN_TAIL:
		obj = new CHitEffect(HIT_EFFECT_TURN_TAIL);
		break;
	case OBJECT_TYPE_HIT_EFFECT_FIRE_BULLET:
		obj = new CHitEffect(HIT_EFFECT_FIRE_BULLET);
		break;
	case OBJECT_TYPE_SPECIAL_ITEM:
	{
		obj = new CSpecialItem();
	}
	break;
	case OBJECT_TYPE_SCORE_AND_1LV: obj = new CScoreUp(); break;
		/*case OBJECT_TYPE_CAMERA:
		{
			camera = new CCamera(x, y, ani_set_id);
			cameras.push_back(camera);
			break;
		}*/
	case OBJECT_TYPE_BREAKABLE_BRICK_ANIMATION_TYPE_LEFT_TOP:
		obj = new CBreakableBrickAnimation(BREAKABLE_BRICK_ANIMATION_TYPE_LEFT_TOP);
		break;
	case  OBJECT_TYPE_BREAKABLE_BRICK_ANIMATION_TYPE_RIGHT_TOP:
		obj = new CBreakableBrickAnimation(BREAKABLE_BRICK_ANIMATION_TYPE_RIGHT_TOP);
		break;
	case  OBJECT_TYPE_BREAKABLE_BRICK_ANIMATION_TYPE_RIGHT_BOTTOM:
		obj = new CBreakableBrickAnimation(BREAKABLE_BRICK_ANIMATION_TYPE_RIGHT_BOTTOM);
		break;
	case OBJECT_TYPE_BREAKABLE_BRICK_ANIMATION_TYPE_LEFT_BOTTOM:
		obj = new CBreakableBrickAnimation(BREAKABLE_BRICK_ANIMATION_TYPE_LEFT_BOTTOM);
		break;
	case OBJECT_TYPE_QUESTION_BRICK_MULTI_COIN:
		obj = new CQuestionBrick(QUESTION_BRICK_HAVE_MULTI_COIN);
		break;
	case OBJECT_TYPE_FLOATING_WOOD:
	{
		obj = new CFloatingWood();
	}
	break;
	case OBJECT_TYPE_BOOMERANG_ENEMY:
		obj = new CBoomerangEnemy();
		break;
	case OBJECT_TYPE_BOOMERANG:
	{
		int boomerang_id = atof(tokens[4].c_str());
		obj = new CBoomerang(boomerang_id);
	}
	break;
	case OBJECT_TYPE_HUD:
	{
		obj = HudPanel::GetInstance();
		if (obj != NULL)
		{
			LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
			obj = new HudPanel();
			obj->SetPosition(x, y);
			obj->SetAnimationSet(ani_set);
			obj->SetOrigin(x, y, obj->GetState());
			obj->SetisOriginObj(true);
			objects.push_back(obj);
			return;
		}


	}

	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	

	if (obj != NULL)
	{
		if (obj != NULL)
		{
			obj->SetPosition(x, y);
			obj->SetAnimationSet(ani_set);
			obj->SetOrigin(x, y, obj->GetState());
			obj->SetisOriginObj(true);
			objects.push_back(obj);
		}
	}
	
	//// add 1 cartridge_clip
	if (dynamic_cast<CFireBullet *>(obj))
	{
		cartridge_clip.push_back(obj);
	}
	if (dynamic_cast<CScoreUp *>(obj))
	{
		score_list.push_back(obj);
	}
	if (dynamic_cast<CHitEffect *>(obj))
	{
		hit_effect_list.push_back(obj);
	}

}
//float CPlayScene::UpdateCamMoveX(DWORD dt)
//{
//
//	float cam_x_end_temp = cameras.at(0)->GetEndCamX();
//
//	float cam_x_game = CGame::GetInstance()->GetCamPosX();
//
//	if (cam_x_game < cam_x_end_temp)
//	{
//		cam_x_game += MOVE_CAM_X_VX * dt;
//		return cam_x_game;
//	}
//	else
//	{
//		return cam_x_end_temp;
//	}
//
//
//}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[MAP]") { section = SCENE_SECTION_MAP; continue; }
		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[GRID]") {
			section = SCENE_SECTION_GRID; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }


		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		case SCENE_SECTION_GRID: _ParseSection_GRID(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	bool isTransform = player->GetIsTransforming();
	vector<LPGAMEOBJECT> coObjects;
	CGame *game = CGame::GetInstance();
	int id = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetId();
	if (id != ID_MAP_4)
	{
		float rangeXleft = player->x - game->GetScreenHeight() / 2 - 100;
		float rangeXright = player->x + game->GetScreenHeight() / 2 + 100;
		for (size_t i = 0; i < objects.size(); i++)
		{
			if (!dynamic_cast<CNoCollisionObjects *>(objects[i]))
			{

				if ((objects[i]->x > rangeXleft &&
					objects[i]->x < rangeXright) || dynamic_cast<HudPanel *>(objects[i]) || dynamic_cast<CFireBullet *>(objects[i]) || dynamic_cast<CFlowerBullet *>(objects[i]) || dynamic_cast<CBoomerang *>(objects[i]) || dynamic_cast<CHitEffect *>(objects[i]))
					coObjects.push_back(objects[i]);
			}

		}

		for (size_t i = 0; i < objects.size(); i++)
		{

			if ((objects[i]->x > rangeXleft &&
				objects[i]->x < rangeXright) || dynamic_cast<HudPanel *>(objects[i]) || dynamic_cast<HudPanel *>(objects[i]) || dynamic_cast<CFireBullet *>(objects[i]) || dynamic_cast<CFlowerBullet *>(objects[i]) || dynamic_cast<CBoomerang *>(objects[i]) || dynamic_cast<CHitEffect *>(objects[i]))
			{
				if (dynamic_cast<CMario*>(objects[i]))
				{
					objects[i]->Update(dt, &coObjects);
				}
				else if (!isTransform)
				{
					objects[i]->Update(dt, &coObjects);
				}
			}

		}
	}
	else
	{

		float cx = game->GetCamPosX();

		float cy = game->GetCamPosY();

		for (size_t i = 0; i < objects.size(); i++)
		{
			float Ox, Oy;
			objects[i]->GetPosition(Ox, Oy);
			if (!IsInUseArea(Ox, Oy) && !objects[i]->GetisOriginObj())
			{
				objects[i]->SetActive(false);
				objects.erase(objects.begin() + i);
			}
		}
		/*player->GetPosition(cx, cy);*/
		grid->GetObjects(objects, cx, cy);
		for (size_t i = 0; i < objects.size(); i++)
		{

			if (!player->GetIsTransforming())
			{
				/*if (!dynamic_cast<CNoCollisionObjects*>(objects[i]))*/
					objects[i]->Update(dt, &objects);
			}
			else
			{
				if (dynamic_cast<CMario*>(objects[i]) || dynamic_cast<HudPanel *>(objects[i]) || dynamic_cast<CFireBullet *>(objects[i]) || dynamic_cast<CFlowerBullet *>(objects[i]) || dynamic_cast<CBoomerang *>(objects[i]) || dynamic_cast<CHitEffect *>(objects[i])|| dynamic_cast<CHitEffect *>(objects[i])|| dynamic_cast<CSpecialItem *>(objects[i]))
					objects[i]->Update(dt, &objects);
			}
		}

		// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
		if (player == NULL) return;
		// chống đi lùi màn hình
		if (player->x < 0) player->x = 0;
		//// Update camera to follow mario

	}



}

void CPlayScene::Render()
{
	if (map)
	{
		this->map->Render();
	}
	CGame *game = CGame::GetInstance();
	float rangeXleft = player->x - game->GetScreenHeight() - 200;
	float rangeXright = player->x + game->GetScreenHeight() + 200;
	for (unsigned int i = 1; i < objects.size(); i++)
	{

		if ((objects[i]->x > rangeXleft &&
			objects[i]->x < rangeXright))
			objects[i]->Render();
	}

}

/*
	Unload current scene
*/
void CPlayScene::_ParseSection_GRID(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring file_path = ToWSTR(tokens[0]);

	if (grid == NULL)
		grid = new CGrid(file_path.c_str());
}

void CPlayScene::Unload()
{
	DebugOut(L"[INFO] Vô undload nè\n");
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	//save GAME info
	CGame *game = CGame::GetInstance();
	if (mario != NULL)
	{
		game->SetCoinCounter(mario->GetCoinCounter());

		game->SetLifeCounter(mario->GetLifeCounter());
		game->SetScore(mario->GetScore());
		game->SetItemList(mario->GetItemList());
		game->SetMarioLevel(mario->GetLevel());

	}

	for (unsigned int i = 0; i < objects.size(); i++)
	{
		if (!dynamic_cast<HudPanel *>(objects[i]))
			delete objects[i];
	}

	delete map;

	map = nullptr;

	objects.clear();
	cartridge_clip.clear();
	score_list.clear();
	hit_effect_list.clear();
	player = NULL;

	grid->Unload();

	grid = nullptr;

	delete grid;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}
bool CPlayScene::IsInUseArea(float Ox, float Oy)
{
	float CamX, CamY;

	CamX = CGame::GetInstance()->GetCamPosX();

	CamY = CGame::GetInstance()->GetCamPosY();

	if (((CamX < Ox + 50) && (Ox < CamX + IN_USE_WIDTH)) && ((CamY < Oy) && (Oy < CamY + IN_USE_HEIGHT)))
		return true;
	return false;
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CGame *game = CGame::GetInstance();
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->GetState() == MARIO_STATE_DIE || mario->GetState() == MARIO_STATE_PIPE_SLIDE_DOWN || mario->GetState() == MARIO_STATE_PIPE_SLIDE_UP || mario->GetAutoWalk() || mario->GetIsTransforming()) return;
	switch (KeyCode)
	{
	case DIK_S:
	{
		if (mario->GetJumpingState() == false) // normal
		{
			mario->SetState(MARIO_STATE_JUMP);
			mario->SetJumpingState(true);
		}

	}

	break;
	case DIK_Q:
		mario->Reset();
		break;
		/*case DIK_A:
			if (game->IsKeyDown(DIK_RIGHT))
			{
				mario->SetState(MARIO_STATE_RUNNING_RIGHT);
			}
			else if (game->IsKeyDown(DIK_LEFT))
			{
				mario->SetState(MARIO_STATE_RUNNING_LEFT);
			}
			break;*/
	case DIK_L:
		mario->SetOnSpecialPosition(4);
		break;
	case DIK_H:
		mario->SetOnSpecialPosition(1);
		break;
	case DIK_J:
		mario->SetOnSpecialPosition(2);
		break;
	case DIK_K:
		mario->SetOnSpecialPosition(3);
		break;
	case DIK_F:
		mario->SetLevel(MARIO_LEVEL_FIRE);

		break;
	case DIK_T:

		mario->SetLevel(MARIO_LEVEL_TAIL);

		break;
	case DIK_B:

		mario->SetLevel(MARIO_LEVEL_BIG);

		break;
	case DIK_M:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;

	case DIK_A:
		mario->SetIsHolding(true);
		if (mario->GetLevel() == MARIO_LEVEL_FIRE)
		{
			mario->Fire();
			mario->SetIsFiring(true);
			break;
			/*mario->StartFiring();*/
		}


		if (mario->GetLevel() == MARIO_LEVEL_TAIL || mario->GetIsTurning() == true)
		{
			mario->SetIsTurning(true);
			mario->StartTurning();
		}

		break;
	case DIK_N:
	{
		CGame::GetInstance()->SwitchScene(2);
		break;
	}
	case DIK_U:
		mario->SetOnSpecialPosition(6);
		break;
	case DIK_I:
		mario->SetOnSpecialPosition(7);
		break;
	case DIK_O:
		mario->SetOnSpecialPosition(8);
		break;
	case DIK_Y:
		mario->SetOnSpecialPosition(5);
		break;
	case DIK_P:
		mario->SetOnSpecialPosition(9);
		break;
	case DIK_Z:
		mario->SetOnSpecialPosition(10);
		break;


	}
}
void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	CGame *game = CGame::GetInstance();
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	if (mario->GetState() == MARIO_STATE_DIE || mario->GetState() == MARIO_STATE_PIPE_SLIDE_DOWN || mario->GetState() == MARIO_STATE_PIPE_SLIDE_UP || mario->GetAutoWalk() || mario->GetIsTransforming()) return;

	switch (KeyCode)
	{
	case DIK_A:
		mario->SetIsHolding(false);
		mario->SetIsHoldAni(false);
		mario->SetIsFiring(false);
		/*mario->StartFiring();*/
		break;




		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();

	// disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DIE || mario->GetAutoWalk() || mario->GetState() == MARIO_STATE_PIPE_SLIDE_DOWN || mario->GetState() == MARIO_STATE_PIPE_SLIDE_UP || mario->GetIsTransforming()) return;

	if (game->IsKeyDown(DIK_RIGHT))
	{
		int current_time = GetTickCount();
		if (game->IsKeyDown(DIK_A))
		{
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
			SetLevelSpeedUp(mario);

		}
		else
		{
			SetLevelSpeedDown(mario);
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
		}
	}

	else if (game->IsKeyDown(DIK_LEFT))
	{

		if (game->IsKeyDown(DIK_A))
		{
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
			SetLevelSpeedUp(mario);
		}
		else
		{
			SetLevelSpeedDown(mario);
			mario->SetState(MARIO_STATE_WALKING_LEFT);
		}

	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		SetLevelSpeedDown(mario);
		if (mario->GetLevel() != MARIO_LEVEL_SMALL)
			mario->SetState(MARIO_STATE_SITDOWN);
		else
		{
			mario->SetState(MARIO_STATE_IDLE);
		}
		if (mario->GetCanPipeSlideDown())
		{
			mario->SetState(MARIO_STATE_PIPE_SLIDE_DOWN);
			mario->StartPipeSlideDown();
			return;
		}

	}
	else if (game->IsKeyDown(DIK_UP))
	{
		SetLevelSpeedDown(mario);
		if (mario->GetCanPipeSlideUp())
		{
			mario->SetState(MARIO_STATE_PIPE_SLIDE_UP);
			mario->StartPipeSlideUp();
			return;
		}
	}
	else // no keystate
	{

		if ((mario->vx <= 0 && mario->nx > 0) || (mario->vx >= 0 && mario->nx < 0))
		{
			SetLevelSpeedDown(mario);
			mario->SetState(MARIO_STATE_IDLE);
		}
		else if (mario->vx != 0)
		{
			SetLevelSpeedDown(mario);
			mario->SetState(MARIO_STATE_SPEEDING_DOWN);
		}
	}

	if (game->IsKeyDown(DIK_S))
	{
		if (mario->GetLevel() == MARIO_LEVEL_TAIL) // fly
		{
			if (mario->GetLevelSpeedUp() == MAX_POWER_SPEED_UP)
			{
				if (mario->GetCanFly() == true)
				{
					mario->StartFlying();
					mario->SetState(MARIO_STATE_FLY);

				}
			}
			//


		}
		//else
		//{
		//	if (mario->GetJumpingState() == false) // normal
		//	{
		//		mario->SetState(MARIO_STATE_JUMP);
		//		mario->SetJumpingState(true);
		//	}
		//}

	}
	else if (game->IsKeyDown(DIK_X))
	{
		if (mario->GetLevel() == MARIO_LEVEL_TAIL)
			if (mario->GetIsJumping() || mario->GetIsLanding()) // on air
				mario->SetState(MARIO_STATE_FALL_DOWN);

	}

}
void CPlayScenceKeyHandler::SetLevelSpeedUp(CMario *mario)
{
	if (mario->GetIsLanding()) return;
	int current_time = GetTickCount();
	// if starttime =0 ..begin to speedup....after 1s...speed up one level and set back starttime..max 7 level ..
	if (start_time_speed_up == 0) // begin to run 
	{
		start_time_speed_up = current_time;
	}
	else // have run for a time
	{
		if (current_time - start_time_speed_up > TIME_PER_LEVEL_SPEED_UP && mario->GetLevelSpeedUp() < MAX_POWER_SPEED_UP)
		{
			//reset start_time_speed_up
			start_time_speed_up = 0;
			mario->IncreaseCurrentLevelSpeed();
		}
	}
}
void CPlayScenceKeyHandler::SetLevelSpeedDown(CMario *mario)
{
	if (mario->GetState() == MARIO_STATE_FLY) return;
	int current_time = GetTickCount();
	// if starttime =0 ..begin to speedup....after 1s...speed up one level and set back starttime..max 7 level ..
	if (start_time_speed_down == 0) // begin to run 
	{
		start_time_speed_down = current_time;
	}
	else // have run for a time
	{
		if (current_time - start_time_speed_down > TIME_PER_LEVEL_SPEED_UP)
		{
			//reset start_time_speed_up
			start_time_speed_down = 0;
			if (mario->GetLevelSpeedUp() > 0) // count down the lv speed
				mario->DecreaseCurrentLevelSpeed();
		}
	}
}