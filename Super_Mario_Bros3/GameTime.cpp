#include "GameTime.h"
GameTime::GameTime()
{
	for (int i = 0; i < 3; i++)
	{
		panel_numbers.push_back(new Number());
		/*panel_numbers[i]->SetPosition(57 + 8 * i, 17);*/
		panel_numbers[i]->SetPosition(129 + 8 * i, 17);
	
	}


	resetValues();
}

void GameTime::Update()
{

	/*if (hundreds == 0 && dozens == 0 && units == 0)
	{
		Game::instance()->stopGameTime();
		if (Game::instance()->getCurState() != "END_OF_LEVEL")
			Game::instance()->gameover();
		return;
	}
	if (dozens == 0 && units == 0)
	{
		if (Game::instance()->getCurState() != "END_OF_LEVEL")
		{
			if (hundreds == 1)
				Game::instance()->hurryUp();
		}

		dozens = 10;
		panel_numbers[0]->setPixmap(numbers[--hundreds]);
	}
	if (units == 0)
	{
		units = 10;
		panel_numbers[1]->setPixmap(numbers[--dozens]);
	}

	panel_numbers[2]->setPixmap(numbers[--units]);*/
}
void GameTime::Render()
{

}
void GameTime::resetValues()
{
	units = 0;
	dozens = 0;
	hundreds = 3;
	/*panel_numbers[0]->setPixmap(numbers[hundreds]);
	panel_numbers[1]->setPixmap(numbers[dozens]);
	panel_numbers[2]->setPixmap(numbers[units]);*/
}

void GameTime::reset()
{
	resetValues();
	/*HudSubPanel::reset();*/
}