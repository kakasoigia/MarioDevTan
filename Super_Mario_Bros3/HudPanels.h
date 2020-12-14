#pragma once

#include <map>
#include "HudSubPanels.h"
#include "GameObject.h"

#define HUD_SPRITE_PANEL_INFO	60100
#define HUD_SPRITE_PANEL_ITEM	60101
#define HUD_SPRITE_ICON_MARIO	60102
#define HUD_SPRITE_ICON_LUGI	60103
#define HUD_SPRITE_POWERMELTER_FILLED_ARROW	60104
#define HUD_SPRITE_POWERMELTER_FILLED_LIGHT	60105
#define HUD_SPRITE_POWERMELTER_EMPTY_ARROW	60106
#define HUD_SPRITE_POWERMELTER_EMPTY_LIGHT	60107
#define HUD_SPRITE_EMPTY_CARD	60108
#define HUD_SPRITE_MUSHROOM_CARD	60109
#define HUD_SPRITE_FLOWER_CARD	60110
#define HUD_SPRITE_STAR_CARD	60111
#define HUD_SPRITE_0	60112
#define HUD_SPRITE_1	60113
#define HUD_SPRITE_2	60114
#define HUD_SPRITE_3	60115
#define HUD_SPRITE_4	60116
#define HUD_SPRITE_5	60117
#define HUD_SPRITE_6	60118
#define HUD_SPRITE_7	60119
#define HUD_SPRITE_8	60120
#define HUD_SPRITE_9	60121
#define HUD_SPRITE_A	60122
#define HUD_SPRITE_B	60123
#define HUD_SPRITE_C	60124
#define HUD_SPRITE_D	60125
#define HUD_SPRITE_E	60126
#define HUD_SPRITE_F	60127
#define HUD_SPRITE_G	60128
#define HUD_SPRITE_H	60129
#define HUD_SPRITE_I	60130
#define HUD_SPRITE_J	60131
#define HUD_SPRITE_K	60132
#define HUD_SPRITE_L	60133
#define HUD_SPRITE_M	60134
#define HUD_SPRITE_N	60135
#define HUD_SPRITE_O	60136
#define HUD_SPRITE_P	60137
#define HUD_SPRITE_Q	60138
#define HUD_SPRITE_R	60139
#define HUD_SPRITE_S	60140
#define HUD_SPRITE_T	60141
#define HUD_SPRITE_U	60142
#define HUD_SPRITE_V	60143
#define HUD_SPRITE_W	60144
#define HUD_SPRITE_X	60145
#define HUD_SPRITE_Y	60146
#define HUD_SPRITE_Z	60147
#define HUD_SPRITE_BLACK_BACKGROUND	60148





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

public:

	HudPanel();
	static HudPanel* GetInstance();
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void reset();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	LPSPRITE GetSprite(char a);
	vector<LPSPRITE> StringToSprite(string str);
};