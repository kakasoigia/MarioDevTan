#pragma once

#include <map>
#include "HudSubPanels.h"
#include "GameObject.h"
#include "define.h"




class HudPanel : public CGameObject
{
private:
	static HudPanel * __instance;
	/*CGameObject texture_panel;*/
	LPSPRITE background;
	LPSPRITE hudInfo;
	LPSPRITE hudItem;
	LPSPRITE playertypeSprite;
	vector<LPSPRITE> lifecountSprite;
	vector<LPSPRITE> coinSprite;
	LPSPRITE worldSprite;
	vector<LPSPRITE> scoreSprite;
	vector<LPSPRITE> gameTimeSprites;
	vector<LPSPRITE> powerMelterSprite;
	vector<LPSPRITE> filledPowerMelterSprite;
	vector<LPSPRITE> itemSprite;
	int life_count = 4;
	int game_time = 0;
	int score = 0;
	int coin = 0;
	int world = 1;
	int time = 0;
	int powerMelterStack = 0;
	vector <int> itemList;
public:

	HudPanel();
	static HudPanel* GetInstance();
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void reset();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	LPSPRITE GetSprite(char a);
	vector<LPSPRITE> StringToSprite(string str);
	vector<LPSPRITE>  ItemToSprite(vector<int> itemList);
};