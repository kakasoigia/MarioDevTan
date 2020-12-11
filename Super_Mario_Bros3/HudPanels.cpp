#include "HudPanels.h"
#include "Score.h"
#include "Utils.h"
#include "Game.h"
#include "Mario.h"
#include "PlayScence.h"
#include "Scence.h"
#include "GameTime.h"
#include "LifeCounter.h"
#include "CoinCounter.h"
#include "PowerMeter.h"
#include "Game.h"
#include "Sprites.h"
#include "Mario.h"
#include "GameObject.h"
#include "Scence.h"
#include <string>
#include "PlayScence.h"
HudPanel * HudPanel::__instance = NULL;
HudPanel::HudPanel()
{
	
	CSprites* sprites = CSprites::GetInstance();
	CGame* game = CGame::GetInstance();
	
	hudInfo = sprites->Get(HUD_SPRITE_PANEL_INFO);
	hudItem = sprites->Get(HUD_SPRITE_PANEL_ITEM);
	background = sprites->Get(HUD_SPRITE_BLACK_BACKGROUND);
	worldSprite = GetSprite('1');
	
	playertypeSprite = sprites->Get(HUD_SPRITE_ICON_MARIO);
	for (unsigned int i = 0; i < 7; i++)
	{
		if (i != 6)
			powerMelterSprite.push_back(sprites->Get(HUD_SPRITE_POWERMELTER_EMPTY_ARROW));
		else
			powerMelterSprite.push_back(sprites->Get(HUD_SPRITE_POWERMELTER_EMPTY_LIGHT));
	}
	for (unsigned int i = 0; i < 7; i++)
	{
		if (i != 6)
			filledPowerMelterSprite.push_back(sprites->Get(HUD_SPRITE_POWERMELTER_FILLED_ARROW));
		else
			filledPowerMelterSprite.push_back(sprites->Get(HUD_SPRITE_POWERMELTER_FILLED_LIGHT));
	}
	
	powerMelterStack = 0;
	coin = game->GetCoinCounter();
	score = game->GetScore();
	life_count = game->GetLifeCounter();
}



void HudPanel::reset()
{
	powerMelterStack = 0;
	coin = 0;
	score = 0;
	life_count = 0;
}



void HudPanel::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	
	CGame *game = CGame::GetInstance();
	this->SetPosition(game->GetCamPosX() , game->GetCamPosY() + 310);
	CMario* mario = ((CPlayScene*)game->GetCurrentScene())->GetPlayer();
	CScene* scene = game->GetCurrentScene();
	if (dynamic_cast<CPlayScene*>(scene)) //if it's playscence and not Worldmap
	{
		
		if (mario->GetState() == MARIO_STATE_DIE) return;
		//update new info
		powerMelterStack = mario->GetLevelSpeedUp();
		coin = mario->GetCoinCounter();
		score = mario->GetScore();
		life_count = mario->GetLifeCounter();

		time += dt;
		game_time = 300 - time / 1000;
		if (game_time == 0)
			mario->SetState(MARIO_STATE_DIE);
		//update GAME info
		game->SetCoinCounter(coin) ;
		game->SetLifeCounter(life_count);
		game->SetScore(score);
	}
	
	//update sprite value
	string life_count_str = to_string(life_count);
	while (life_count_str.length() < 2) life_count_str = "0" + life_count_str;
	lifecountSprite = StringToSprite(life_count_str);

	string time_str = to_string(game_time);
	while (time_str.length() < 3) time_str = "0" + time_str;
	gameTimeSprites = StringToSprite(time_str);

	string score_str = to_string(score);
	while (score_str.length() < 7) score_str = "0" + score_str;
	scoreSprite = StringToSprite(score_str);

	string coin_str = to_string(coin);
	coinSprite = StringToSprite(coin_str);
	
}
void HudPanel::Render()
{
	
	//draw a black background
	/*background->Draw(x, y-78);*/
	
	//draw info
	hudInfo->Draw( x + 40, y - 78);
	
	hudItem->Draw( x + 220, y - 78);
	
	playertypeSprite->Draw( x + 44, y - 63);

	worldSprite->Draw( x + 77, y - 71);
	
	for (unsigned int i = 0; i < lifecountSprite.size(); i++)
	{
		
		lifecountSprite[i]->Draw( x + 69 + 8 * i, y - 63);
	}
	
	for (unsigned int i = 0; i < gameTimeSprites.size(); i++)
	{
		gameTimeSprites[i]->Draw( x + 164 + 8 * i, y - 63);
	}
	
	for (unsigned int i = 0; i < scoreSprite.size(); i++)
	{
		scoreSprite[i]->Draw( x + 92 + 8 * i, y - 63);
	}
	
	for (unsigned int i = 0; i < coinSprite.size(); i++)
	{
		coinSprite[i]->Draw( x + 172 + 8 * i, y - 71);
	}

	for (unsigned int i = 0; i < powerMelterSprite.size(); i++)
	{
		powerMelterSprite[i]->Draw( x + 92 + 8 * i, y - 71);
	}
	
	/*for (int i = 0; i < powerMelterStack; i++)
	{
		filledPowerMelterSprite[i]->Draw( x + 92 + 8 * i, y - 71);
	}*/
	
}
void HudPanel::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	
}
LPSPRITE HudPanel::GetSprite(char a)
{
	CSprites * sprites = CSprites::GetInstance();
	switch (a)
	{
	case '0':
		return sprites->Get(HUD_SPRITE_0);
		break;
	case '1':
		return sprites->Get(HUD_SPRITE_1);
		break;
	case '2':
		return sprites->Get(HUD_SPRITE_2);
		break;
	case '3':
		return sprites->Get(HUD_SPRITE_3);
		break;
	case '4':
		return sprites->Get(HUD_SPRITE_4);
		break;
	case '5':
		return sprites->Get(HUD_SPRITE_5);
		break;
	case '6':
		return sprites->Get(HUD_SPRITE_6);
		break;
	case '7':
		return sprites->Get(HUD_SPRITE_7);
		break;
	case '8':
		return sprites->Get(HUD_SPRITE_8);
		break;
	case '9':
		return sprites->Get(HUD_SPRITE_9);
		break;
	default:
		return NULL;
		break;
	}
}
vector<LPSPRITE> HudPanel::StringToSprite(string str)
{
	vector<LPSPRITE> sprites;
	LPSPRITE sprite;
	char c;
	for (int i = 0; i < str.size(); i++)
	{
		c = (char)str[i];
		sprite = GetSprite(c);
		if (sprite != NULL)
			sprites.push_back(sprite);
	}
	return sprites;
}
HudPanel *HudPanel::GetInstance()
{
	if (__instance == NULL) __instance = new HudPanel();
	return __instance;
}