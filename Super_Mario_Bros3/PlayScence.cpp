#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "BreakableBrick.h"
#include "Bell.h"
#include "Leaf.h"
#include "MushRoom.h"
#include "HudPanels.h"
#include "HudSubPanels.h"
#include "SpecialItem.h"
using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6

#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS_GREEN_WALK	3
#define OBJECT_TYPE_RECTANGLE	5
#define OBJECT_TYPE_NO_COLLISION_OBJECTS 4
#define OBJECT_TYPE_PIPE 6
#define OBJECT_TYPE_KOOPAS_GREEN_FLY	7
#define OBJECT_TYPE_KOOPAS_RED_WALK	8
#define OBJECT_TYPE_KOOPAS_RED_FLY	9
#define OBJECT_TYPE_COIN_NORMAL	10
#define OBJECT_TYPE_GOOMBA_RED_FLY	11
#define OBJECT_TYPE_FIRE_BULLET	12
#define OBJECT_TYPE_FLOWER_RED		13
#define OBJECT_TYPE_FLOWER_BULLET	14
#define OBJECT_TYPE_QUESTION_BRICK_NORMAL	15
#define OBJECT_TYPE_LEAF	16
#define OBJECT_TYPE_MUSHROOM_RED		17
#define OBJECT_TYPE_QUESTION_BRICK_HAVE_LEAF	18
#define OBJECT_TYPE_MUSHROOM_GREEN		19
#define OBJECT_TYPE_QUESTION_BRICK_JUST_HAVE_MUSHROOM	20
#define OBJECT_TYPE_FLOWER_GREEN				21
#define OBJECT_TYPE_FLOWER_GREEN_CAN_SHOOT		22
#define OBJECT_TYPE_BREAKABLE_BRICK_NORMAL	23
#define OBJECT_TYPE_BREAKABLE_BRICK_BELL	25
#define OBJECT_TYPE_BELL	24
#define OBJECT_TYPE_COIN_CAN_TOSS	26
#define OBJECT_TYPE_HUD	27
#define OBJECT_TYPE_SPECIAL_ITEM	28
#define OBJECT_TYPE_PORTAL	50
 
#define MAX_SCENE_LINE 1024
#define MAX_POWER_SPEED_UP 7
#define	TIME_PER_LEVEL_SPEED_UP 200


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
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	double x = atof(tokens[1].c_str());
	double y = atof(tokens[2].c_str());

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
	case OBJECT_TYPE_NO_COLLISION_OBJECTS:obj = new CNoCollisionObjects(); break;
	case OBJECT_TYPE_COIN_NORMAL:obj = new CCoin(COIN_NORMAL); break;
	case OBJECT_TYPE_COIN_CAN_TOSS:obj = new CCoin(COIN_CAN_TOSS); break;
	case OBJECT_TYPE_PIPE:obj = new CPipe(); break;
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
	case OBJECT_TYPE_SPECIAL_ITEM: obj = new CSpecialItem(); break;
	case OBJECT_TYPE_HUD: 
	{
		obj = HudPanel::GetInstance();
		if (obj != NULL)
		{
			obj = new HudPanel();
			objects.push_back(obj);
			return;
		}
	
		
	}

	case OBJECT_TYPE_PORTAL:
	{
		double r = atof(tokens[4].c_str());
		double b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);


	objects.push_back(obj);
	//// add 1 cartridge_clip
	if (dynamic_cast<CFireBullet *>(obj))
	{
		cartridge_clip.push_back(obj);
	}

}

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

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (!dynamic_cast<CNoCollisionObjects *>(objects[i]))
			coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		CGame *game = CGame::GetInstance();
		float rangeXleft = player->x - game->GetScreenHeight() - 100;
		float rangeXright = player->x + game->GetScreenHeight() + 100;
		if (objects[i]-> x>rangeXleft && 
			objects[i]->x< rangeXright)
		objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;
	// chống đi lùi màn hình
	if (player->x < 0) player->x = 0;
	//// Update camera to follow mario
	//float cx, cy;
	//player->GetPosition(cx, cy);

	//CGame *game = CGame::GetInstance();
	//cx -= game->GetScreenWidth() / 2;
	//cy -= game->GetScreenHeight() / 2;

	//float camX = 0;
	//float camY = 0;
	//if (player->x > (game->GetScreenWidth() / 2)) camX = cx;
	//if (player->GetState()== MARIO_STATE_FLY || player->GetState() == MARIO_STATE_FALL_DOWN || player-> GetIsLanding()==true || player->y<10)
	//	if (player->y <= (game->GetScreenHeight() / 2)) camY =cy;
	//CGame::GetInstance()->SetCamPos((int)camX, (int)camY);


}

void CPlayScene::Render()
{
	
	for (unsigned int i = 1; i < objects.size(); i++)
	{
			objects[i]->Render();
	}
		
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (unsigned int i = 0; i < objects.size(); i++)
	{
		if(!dynamic_cast<HudPanel *>(objects[i]))
		delete objects[i];
	}
		

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CGame *game = CGame::GetInstance();
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->GetState() == MARIO_STATE_DIE) return;
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
	case DIK_F:
		mario->SetLevel(MARIO_LEVEL_FIRE);
		break;
	case DIK_T:
		mario->SetLevel(MARIO_LEVEL_TAIL);
		break;
	case DIK_SPACE:
		if (mario->GetLevel() == MARIO_LEVEL_FIRE)
		{
			mario->Fire();
			mario->SetIsFiring(true);
			/*mario->StartFiring();*/
		}

		break;
	case DIK_Z:
		if (mario->GetLevel() == MARIO_LEVEL_TAIL || mario->GetIsTurning() == true)
		{
			mario->SetIsTurning(true);
			mario->StartTurning();
		}

		break;
	case DIK_P:
	{
			CGame::GetInstance()->SwitchScene(2);
		     break;
	}
		
	}
}
void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CGame *game = CGame::GetInstance();
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->GetState() == MARIO_STATE_DIE) return;
	switch (KeyCode)
	{
	case DIK_A:
		mario->SetIsHolding(false);
		mario->SetIsHoldAni(false);
		break;
	case DIK_SPACE:
		
			mario->SetIsFiring(false);
			/*mario->StartFiring();*/
		

		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();

	// disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DIE || mario->GetAutoWalk()) return;
	
	if (game->IsKeyDown(DIK_RIGHT))
	{
		int current_time = GetTickCount();
		if (game->IsKeyDown(DIK_A))
		{
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
			SetLevelSpeedUp(mario);
			mario->SetIsHolding(1);
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
		mario->SetState(MARIO_STATE_SITDOWN);
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

	if (game->IsKeyDown(DIK_X))
	{
		if ( mario->GetLevel() == MARIO_LEVEL_TAIL) // fly
		{
			if (mario->GetLevelSpeedUp() == MAX_POWER_SPEED_UP )
			{
				if (mario->GetCanFly() == true)
				{
					mario->StartFlying();
					mario->SetState(MARIO_STATE_FLY);

				}
			}
			else if (mario->GetIsJumping() || mario->GetIsLanding()) // on air
			{
				mario->SetState(MARIO_STATE_FALL_DOWN);
			}
			
				
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