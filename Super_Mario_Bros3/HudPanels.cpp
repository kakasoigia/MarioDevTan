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
HudPanel::HudPanel()
{

	sub_panels["Score"] = new Score();

	/*sub_panels["GameTime"] = new GameTime();*/
	
	sub_panels["CoinCounter"] = new CoinCounter();
	sub_panels["PowerMeter"] = new PowerMeter();
	
	sub_panels["LifeCounter"] = new LifeCounter();
	/*sub_panels["CardsTaken"] = new CardsTaken();*/

	CGame *game = CGame::GetInstance();
	this->SetPosition(game->GetCamPosX() + 20, game->GetCamPosY() + 170);



}



void HudPanel::reset()
{
	
}



void HudPanel::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGame *game = CGame::GetInstance();
	this->SetPosition(game->GetCamPosX() + 20, game->GetCamPosY() + 170);
	//set HUD run behind Mario
	
		// update Score
	string panel_id = "Score";
	Score *score_hud = dynamic_cast<Score *>(sub_panels[panel_id]);
	score_hud->Update(x, y);
	//update CoinCounter
	string panel_id2 = "CoinCounter";
	CoinCounter *coin_counter = dynamic_cast<CoinCounter *>(sub_panels[panel_id2]);
	coin_counter->Update(x, y);
	//update PowerMeter
	string panel_id3 = "PowerMeter";
	PowerMeter *power_meter = dynamic_cast<PowerMeter *>(sub_panels[panel_id3]);
	power_meter->Update(x, y);
	//update LifeCounter
	string panel_id4 = "LifeCounter";
	LifeCounter *life_counter = dynamic_cast<LifeCounter *>(sub_panels[panel_id4]);
	life_counter->Update(x, y);
	/*sub_panels[panel_id]->Update(x, y);*/
}
void HudPanel::Render()
{
	
	
	//render Background Black
	animation_set->at(HUDPANEL_ANI_HUD_BLACK_BACKGROUD)->Render(this->x-20, this->y-3, 255);
	//render BIG HUD
	animation_set->at(HUDPANEL_ANI_BIG_HUD)->Render(this->x, this->y, 255);
	//render world map 1 
	animation_set->at(1)->Render(this->x+38, this->y+7, 255);
	map<string, HudSubPanel*>::iterator i;

	for (i = sub_panels.begin(); i != sub_panels.end(); i++)
		i->second->Render();

}
void HudPanel::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	
}