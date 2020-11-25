#include "HudPanels.h"
#include "Score.h"
#include "Utils.h"
#include "Game.h"
#include "Mario.h"
#include "PlayScence.h"
#include "Scence.h"
HudPanel::HudPanel()
{

	sub_panels["Score"] = new Score();
	sub_panels["Score"]->animation_set = this->animation_set;

	/*sub_panels["GameTime"] = new GameTime();
	sub_panels["CardsTaken"] = new CardsTaken();
	
	sub_panels["LifeCounter"] = new LifeCounter();
	sub_panels["CoinCounter"] = new CoinCounter();
	sub_panels["PowerMeter"] = new PowerMeter();*/
	CGame *game = CGame::GetInstance();
	SetPosition(game->GetCamPosX(),game->GetCamPosY());

}



void HudPanel::reset()
{
	
}



void HudPanel::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{

	//set HUD run behind Mario
	
		DebugOut(L"[INFO] con cac \n");
	
	string panel_id = "Score";
	/*map<std::string, HudSubPanel*>::iterator i;
	for (int i = sub_panels.begin(); i != sub_panels.end(); i++)*/
	
	Score *p = dynamic_cast<Score *>(sub_panels[panel_id]);
	p->Update();

}
void HudPanel::Render()
{
	CGame *game = CGame::GetInstance();
	this->SetPosition(game->GetCamPosX() +20, game->GetCamPosY()+170);

	//render Background Black
	animation_set->at(HUDPANEL_ANI_HUD_BLACK_BACKGROUD)->Render(this->x-20, this->y-3, 255);
	//render BIG HUD
	animation_set->at(HUDPANEL_ANI_BIG_HUD)->Render(this->x, this->y, 255);
	
	string panel_id = "Score";
	/*map<std::string, HudSubPanel*>::iterator i;
	for (int i = sub_panels.begin(); i != sub_panels.end(); i++)*/

	Score *p = dynamic_cast<Score *>(sub_panels[panel_id]);
	p->Render();

}
void HudPanel::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	
}