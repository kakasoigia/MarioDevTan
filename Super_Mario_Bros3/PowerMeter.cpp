#include "PowerMeter.h"
#include "Game.h"
#include "Mario.h"
#include "PlayScence.h"
PowerMeter ::PowerMeter()
{
	CGame *game = CGame::GetInstance();
	//set up number  
	for (int i = 0; i < 7; i++)
	{

		panel_numbers.push_back(new Number());
		/*panel_numbers[i]->SetPosition(57 + 8 * i, 17);*/
		
	}
}
void PowerMeter::Update(float hud_x, float hud_y)
{

	CGame *game = CGame::GetInstance();
	for (int i = 0; i < 6; i++)
	{
		panel_numbers[i]->SetPosition(hud_x + 52 + 8 * i, hud_y + 7);
	}
	panel_numbers[6]->SetPosition(hud_x + 52 + 8 * 6+1, hud_y + 7);
	CMario* player = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	int current_meter = player->GetLevelSpeedUp();
	for (int i = 0; i < current_meter; i++)
	{
		panel_numbers[i]->SetIsMeterUp(true)  ;
	}
	for (int i = current_meter; i < 7; i++)
	{
		panel_numbers[i]->SetIsMeterUp(false);
	}

	

}

void PowerMeter::Render()
{
	//set pos go with mario

	animation_set = CAnimationSets::GetInstance()->Get(HUDPANEL_ANIMATION_SET_ID);
	for (int i = 0; i < 6; i++)
	{
		if(panel_numbers[i] ->GetIsMeterUp() )
			animation_set->at(HUDPANEL_ANI_ARROW_MELTER)->Render(panel_numbers[i]->x, panel_numbers[i]->y, 255);
		
	}
	if (panel_numbers[6]->GetIsMeterUp())
		animation_set->at(HUDPANEL_ANI_LIGHT_MELTER)->Render(panel_numbers[6]->x, panel_numbers[6]->y, 255);

}

void PowerMeter::reset()
{

}
