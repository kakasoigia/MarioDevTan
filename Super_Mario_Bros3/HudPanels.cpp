#include "HudPanels.h"
#include "Score.h"
#include "Utils.h"
#include "Game.h"
#include "Mario.h"
#include "PlayScence.h"
#include "Scence.h"
#include "GameTime.h"
#include "CoinCounter.h"
HudPanel::HudPanel()
{

	sub_panels["Score"] = new Score();

	sub_panels["GameTime"] = new GameTime();
	
	sub_panels["CoinCounter"] = new CoinCounter();
	/*sub_panels["PowerMeter"] = new PowerMeter();
	sub_panels["CardsTaken"] = new CardsTaken();

	sub_panels["LifeCounter"] = new LifeCounter();*/
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
	
		DebugOut(L"[INFO] dell hiểu kiểu gì \n");
		// update Score
	string panel_id = "Score";
	Score *p = dynamic_cast<Score *>(sub_panels[panel_id]);
	p->Update(x, y);
	//update CoinCounter
	string panel_id2 = "CoinCounter";
	CoinCounter *q = dynamic_cast<CoinCounter *>(sub_panels[panel_id2]);
	q->Update(x, y);

}
void HudPanel::Render()
{
	
	
	//render Background Black
	animation_set->at(HUDPANEL_ANI_HUD_BLACK_BACKGROUD)->Render(this->x-20, this->y-3, 255);
	//render BIG HUD
	animation_set->at(HUDPANEL_ANI_BIG_HUD)->Render(this->x, this->y, 255);
	//update Score
	string panel_id = "Score";
	Score *p = dynamic_cast<Score *>(sub_panels[panel_id]);
	p->Render();
	//update CoinCounter
	string panel_id2 = "CoinCounter";
	CoinCounter *q = dynamic_cast<CoinCounter *>(sub_panels[panel_id2]);
	q->Render();

}
void HudPanel::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	
}