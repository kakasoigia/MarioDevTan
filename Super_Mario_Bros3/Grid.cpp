#include "PlayScence.h"


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_MAP	7
#define SCENE_SECTION_GRID	8

#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS_GREEN_WALK	3
#define OBJECT_TYPE_RECTANGLE	5
#define OBJECT_TYPE_NO_COLLISION_OBJECTS 4
#define OBJECT_TYPE_PIPE_NORMAL 6
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
#define OBJECT_TYPE_PIPE_CAN_SLIDE_DOWN 29
#define OBJECT_TYPE_PIPE_CAN_SLIDE_UP 30
#define OBJECT_TYPE_BREAKABLE_BRICK_ANIMATION_TYPE_LEFT_TOP				31
#define OBJECT_TYPE_BREAKABLE_BRICK_ANIMATION_TYPE_RIGHT_TOP			32
#define OBJECT_TYPE_BREAKABLE_BRICK_ANIMATION_TYPE_RIGHT_BOTTOM			33
#define OBJECT_TYPE_BREAKABLE_BRICK_ANIMATION_TYPE_LEFT_BOTTOM			34
#define OBJECT_TYPE_SCORE_AND_1LV		35
#define OBJECT_TYPE_CAMERA		36
#define OBJECT_TYPE_QUESTION_BRICK_MULTI_COIN		48
#define OBJECT_TYPE_FLOATING_WOOD		49
#define OBJECT_TYPE_BOOMERANG											50
#define OBJECT_TYPE_BOOMERANG_ENEMY										51
#define OBJECT_TYPE_HIT_EFFECT_FIRE_BULLET 52
#define	OBJECT_TYPE_HIT_EFFECT_TURN_TAIL 53
#define OBJECT_TYPE_PORTAL	100

#define MAX_SCENE_LINE 1024
#define MAX_POWER_SPEED_UP 7
#define	TIME_PER_LEVEL_SPEED_UP 200



CGrid::CGrid(LPCWSTR filePath)
{
	Load(filePath);
}

void CGrid::_ParseSection_SETTINGS(string line)
{
	vector<string> tokens = split(line);
	DebugOut(L"--> %s\n", ToWSTR(line).c_str());

	if (tokens.size() < 4) return; // skip invalid lines

	cellWidth = atoi(tokens[0].c_str());
	cellHeight = atoi(tokens[1].c_str());
	numCol = atoi(tokens[2].c_str());
	numRow = atoi(tokens[3].c_str());

	cells = new LPCELL[numCol];
	for (int i = 0; i < numCol; i++)
	{
		cells[i] = new Cell[numRow];
	}
}

void CGrid::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);
	DebugOut(L"--> %s\n", ToWSTR(line).c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	if (tokens.size() < 4) return; // skip invalid lines

	int x = atoi(tokens[1].c_str());
	int y = atoi(tokens[2].c_str());

	int cellX = atoi(tokens[4].c_str());
	int cellY = atoi(tokens[5].c_str());

	int type = atoi(tokens[0].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CGameObject* obj = NULL;

	switch (type)
	{
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
			obj = new HudPanel();
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
		DebugOut(L"[ERR] Invalid object type: %d\n", type);
		return;
	}
	// General object setup
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	if (obj != NULL)
	{
		int add = 0;
		obj->SetPosition(x, y);
		obj->SetAnimationSet(ani_set);
		obj->SetOrigin(x, y, obj->GetState());
		cells[cellX][cellY].Add(obj);
		obj->SetisOriginObj(false);
	}

}

void CGrid::Load(LPCWSTR filePath)
{
	DebugOut(L"[INFO] Start loading grid resources from : %s \n", filePath);

	ifstream f;
	f.open(filePath);

	// current resource section flag
	int section;

	char str[MAX_GRID_LINE];
	while (f.getline(str, MAX_GRID_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SETTINGS]") {
			section = GRID_SECTION_SETTINGS; continue;
		}
		if (line == "[OBJECTS]") {
			section = GRID_SECTION_OBJECTS; continue;
		}
		//
		// data section
		//
		switch (section)
		{
		case GRID_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;
		case GRID_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene resources %s\n", filePath);
}

void CGrid::GetObjects(vector<LPGAMEOBJECT>& listObject, int CamX, int CamY)
{
	//listObject.clear();

	int left, top, right, bottom;
	int i, j, k;

	left = ((CamX) / cellWidth);
	right = (CamX + IN_USE_WIDTH) / cellWidth;
	if (((CamX + IN_USE_WIDTH) % cellWidth) != 0)
		right++;
	top = (CamY) / cellHeight;
	bottom = (CamY + IN_USE_HEIGHT) / cellHeight;

	LPGAMEOBJECT obj;

	if (right < 0 || left > numCol || bottom < 0 && top > numRow)
	{
		return;
	}

	if (right > numCol)
	{
		right = numCol;
	}
	if (bottom > numRow)
	{
		bottom = numRow;
	}
	if (left < 0)
	{
		left = 0;
	}
	if (top < 0)
	{
		top = 0;
	}

	for (i = left; i < right; i++)
	{
		for (j = top; j < bottom; j++)
		{
			if (cells)
				if (cells[i][j].GetListObjects().size() > 0)
				{
					for (k = 0; k < cells[i][j].GetListObjects().size(); k++)
					{
						if (!cells[i][j].GetListObjects().at(k)->Actived)
						{
							
							listObject.push_back(cells[i][j].GetListObjects().at(k));

							cells[i][j].GetListObjects().at(k)->SetActive(true);
						}
					}
				}
		}
	}
}

void CGrid::Unload()
{
	if (this != nullptr)
		if (cells)
		{
			for (int i = 0; i < numCol; i++)
			{
				for (int j = 0; j < numRow; j++)
				{
					cells[i][j].Unload();
				}
			}
			delete cells;
			cells = NULL;
		}
}
/*float Ox, Oy;
							cells[i][j].GetListObjects().at(k)->GetOriginLocation(Ox, Oy);*/
							//if (!((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->IsInUseArea(Ox, Oy))// && cells[i][j].GetListObjects().at(k)->GetState() > 10)
													//{
													//	cells[i][j].GetListObjects().at(k)->reset();
													//}
