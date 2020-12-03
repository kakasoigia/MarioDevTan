#include "GameTime.h"
#include "Game.h"
#include "HudPanels.h"
#include "Mario.h"
#include "PlayScence.h"
GameTime::GameTime()
{
	for (int i = 0; i < 3; i++)
	{
		panel_numbers.push_back(new Number());
		/*panel_numbers[i]->SetPosition(57 + 8 * i, 17);*/
		
	
	}


	resetValues();
}

void GameTime::Update(float hud_x,float hud_y)
{
	//update position
	CGame *game = CGame::GetInstance();
	for (int i = 0; i < 3; i++)
	{
		panel_numbers[i]->SetPosition(hud_x + 124 + 8 * i, hud_y + 15);
	}
	CMario* player = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (player->GetState() == MARIO_STATE_DIE) return;
	//update value
	int game_time = CGame::GetInstance()->GetGameTime();
	//time _up
	if (game_time == 0)
	{
		player->SetState(MARIO_STATE_DIE);
		
	}
	hundreds = game_time / 100;
	game_time = game_time % 100;

	dozens = game_time / 10;
	game_time = game_time % 10;

	units = game_time;
	
	
	
	 
	
	
}
void GameTime::Render()
{
	//set pos go with mario

	animation_set = CAnimationSets::GetInstance()->Get(HUDPANEL_ANIMATION_SET_ID);

	
	animation_set->at(hundreds)->Render(panel_numbers[0]->x, panel_numbers[0]->y, 255);
	animation_set->at(dozens)->Render(panel_numbers[1]->x, panel_numbers[1]->y, 255);
	animation_set->at(units)->Render(panel_numbers[2]->x, panel_numbers[2]->y, 255);
}
void GameTime::resetValues()
{
	units = 0;
	dozens = 0;
	hundreds = 3;

}

void GameTime::reset()
{
	resetValues();
	/*HudSubPanel::reset();*/
}