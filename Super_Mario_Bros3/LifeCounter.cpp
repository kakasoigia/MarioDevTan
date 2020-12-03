#include "LifeCounter.h"
#include "Game.h"
#include "Mario.h"
#include "PlayScence.h"
#include "HudPanels.h"
LifeCounter::LifeCounter()
{
	units = 4;
	dozens = 0;
	CGame *game = CGame::GetInstance();
	//set up number  
	for (int i = 0; i < 2; i++)
	{

		panel_numbers.push_back(new Number());
		/*panel_numbers[i]->SetPosition(57 + 8 * i, 17);*/

	}
}
void LifeCounter::Update(float hud_x, float hud_y)
{

	CGame *game = CGame::GetInstance();
	for (int i = 0; i < 2; i++)
	{
		panel_numbers[i]->SetPosition(hud_x + 29 + 8 * i, hud_y + 15);
	}
	
	CMario* player = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	int current_life_counter = player->GetLifeCounter()<100 ? player->GetLifeCounter(): 99;
	dozens = (int)current_life_counter / 10;
	units = current_life_counter % 10;
}

void LifeCounter::Render()
{
	//set pos go with mario

	animation_set = CAnimationSets::GetInstance()->Get(HUDPANEL_ANIMATION_SET_ID);
	if(dozens !=0) // if =0 not render 
	animation_set->at(dozens)->Render(panel_numbers[0]->x, panel_numbers[0]->y, 255);
	animation_set->at(units)->Render(panel_numbers[1]->x, panel_numbers[1]->y, 255);
}

void LifeCounter::reset()
{

}
