#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Rectangle.h"
#include "NoCollisionObjects.h"
#include "Pipe.h"
#include "QuestionBrick.h"
#include "FireBullet.h"
#include "Flower.h"
#include "FlowerBullet.h"
class CPlayScene : public CScene
{
protected:
	CMario *player;					// A play scene has to have player, right? 

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> cartridge_clip;
	vector<LPGAMEOBJECT> score_list;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);


public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	
	CMario * GetPlayer() { return player; }
	vector<LPGAMEOBJECT> Get_cartridge_clip() { return cartridge_clip; };
	vector<LPGAMEOBJECT> Get_objects() { return objects; };
	vector<LPGAMEOBJECT> Get_score_list() { return score_list; };
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

