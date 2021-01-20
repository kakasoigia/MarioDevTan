#pragma once
#include "Game.h"
#include <iostream>
#include <fstream>
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Map.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Rectangle.h"
#include "NoCollisionObjects.h"
#include "Pipe.h"
#include "Camera.h"
#include "QuestionBrick.h"
#include "FireBullet.h"
#include "Flower.h"
#include "Cell.h"
#include "FlowerBullet.h"
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
#include "ScoreUp.h"
#include "BreakableBrickAnimation.h"
#include "Camera.h"
#include "FloatingWood.h"
#include "Boomerang.h"
#include "BoomerangEnemy.h"
#define MOVE_CAM_X_VX		0.1f

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_MAP	7
#define SCENE_SECTION_GRID 8

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
#define OBJECT_TYPE_PORTAL	100

#define MAX_SCENE_LINE 1024
#define MAX_POWER_SPEED_UP 7
#define	TIME_PER_LEVEL_SPEED_UP 200

//GRID

#define IN_USE_WIDTH 330
#define IN_USE_HEIGHT 300
#define CAM_X_BONUS 20
// GRID
#define GRID_SECTION_SETTINGS	1
#define GRID_SECTION_OBJECTS	2
#define MAX_GRID_LINE 1024
class CGrid
{
	int numRow, numCol;
	int cellWidth;
	int  cellHeight;
	Cell** cells;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_OBJECTS(string line);
public:
	CGrid() {}
	CGrid(LPCWSTR filePath);
	void GetObjects(vector<LPGAMEOBJECT>& listObject, int playerX, int playerY);
	void Load(LPCWSTR filePath);
	void Unload();
};
class CPlayScene : public CScene
{
private:

	DWORD time_cam_move = 0;
protected:
	CMario *player;					// A play scene has to have player, right? 

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> cartridge_clip;
	vector<LPGAMEOBJECT> score_list;
	vector<CCamera*> cameras;
	Map* map;
	CGrid* grid;
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);
	void _ParseSection_GRID(string line);
	CCamera* camera = NULL;
	int cam_state = 0;

public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual bool IsInUseArea(float x, float y);
	virtual void Unload();
	float UpdateCamMoveX(DWORD dt);
	CMario * GetPlayer() { return player; }
	vector<LPGAMEOBJECT> Get_cartridge_clip() { return cartridge_clip; };
	vector<LPGAMEOBJECT> Get_objects() { return objects; };
	vector<LPGAMEOBJECT> Get_score_list() { return score_list; };
	void StartTimeCamMove()
	{
		if (time_cam_move == 0)
			time_cam_move = GetTickCount();
	}
	void SetCamState(int camStateInt)
	{
		cam_state = camStateInt;
	}
	int GetCamState()
	{
		return cam_state;
	}
	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) ;
	 void SetLevelSpeedUp(CMario *mario);
	 void SetLevelSpeedDown(CMario *mario);
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
	int start_time_speed_up = 0;
	int start_time_speed_down = 0;
	

};

