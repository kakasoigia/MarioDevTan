#include "CoinCounter.h"
CoinCounter::CoinCounter()
{

	for (int i = 0; i < 2; i++)
	{
			
		panel_numbers.push_back(new Number());
		/*panel_numbers[i]->SetPosition(57 + 8 * i, 17);*/
		panel_numbers[i]->SetPosition(17, 170);
	}
	units = 0;
	dozens = 0;
}

void CoinCounter::Update(float hud_x, float hud_y)
{
	for (int i = 0; i < 2; i++)
	{
		panel_numbers[i]->SetPosition(hud_x + 132 + 8 * i, hud_y + 7);
	}
	int total_coins= CGame::GetInstance()->GetCoinCounter();
	dozens = (int)total_coins/10;
	units = total_coins %10;
}

void CoinCounter::reset()
{
	/*HudSubPanel::reset();*/
}
void CoinCounter::Render()
{
	//set pos go with mario

	animation_set = CAnimationSets::GetInstance()->Get(HUDPANEL_ANIMATION_SET_ID);


	animation_set->at(dozens)->Render(panel_numbers[0]->x, panel_numbers[0]->y, 255);
	animation_set->at(units)->Render(panel_numbers[1]->x, panel_numbers[1]->y, 255);
}