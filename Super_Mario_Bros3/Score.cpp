#include "Score.h"
#include "Score.h"
#include <iostream>
#include "Number.h"
#include "HudSubPanels.h"
#include "HudPanels.h"
#include "Utils.h"
Score::Score() 
{
	//set default number for panels
	units = 5;
	dozens = 5;
	hundreds = 5;
	thousands = 5;
	ten_thousands = 5;
	hundreds_thousands = 5;
	millions = 5;
	CGame *game = CGame::GetInstance();
	//set up number  
	for (int i = 0; i < 7; i++)
	{
	
		panel_numbers.push_back(new Number());
		/*panel_numbers[i]->SetPosition(57 + 8 * i, 17);*/
		panel_numbers[i]->SetPosition(game->GetCamPosX() + 73 + 8 * i, game->GetCamPosY() + 170 + 15);
		
	}

}

void Score::Update(float hud_x,float hud_y)
{

	CGame *game = CGame::GetInstance();
	for (int i = 0; i < 7; i++)
	{
		panel_numbers[i]->SetPosition(hud_x + 52 + 8 * i, hud_y + 15);
	}
	int info = game->GetScore();
	
	millions = info / 1000000;
	info = info % 1000000;

	hundreds_thousands = info / 100000;
	info = info % 100000;

	ten_thousands = info / 10000;
	info = info % 10000;

	thousands = info / 1000;
	info = info % 1000;

	hundreds = info / 100;
	info = info % 100;

	dozens = info / 10;
	info = info % 10;

	units = info;

}

void Score::Render()
{
	//set pos go with mario
	
	animation_set = CAnimationSets::GetInstance()->Get(HUDPANEL_ANIMATION_SET_ID);

	animation_set->at(millions)->Render(panel_numbers[0]->x, panel_numbers[0]->y,255);
	animation_set->at(hundreds_thousands)->Render(panel_numbers[1]->x, panel_numbers[1]->y, 255);
	animation_set->at(ten_thousands)->Render(panel_numbers[2]->x, panel_numbers[2]->y, 255);
	animation_set->at(thousands)->Render(panel_numbers[3]->x, panel_numbers[3]->y, 255);
	animation_set->at(hundreds)->Render(panel_numbers[4]->x, panel_numbers[4]->y, 255);
	animation_set->at(dozens)->Render(panel_numbers[5]->x, panel_numbers[5]->y, 255);
	animation_set->at(units)->Render(panel_numbers[6]->x, panel_numbers[6]->y, 255);
}

void Score::reset()
{
	units = 0;
	dozens = 0;
	hundreds = 0;
	thousands = 0;
	ten_thousands = 0;
	hundreds_thousands = 0;
	millions = 0;

	Render();

	/*HudSubPanel::reset();*/

}
