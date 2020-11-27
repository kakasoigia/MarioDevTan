#pragma once
#include "HudSubPanels.h"
#include "Number.h"
#include "Game.h"
#include "GameObject.h"
#include "HudPanels.h"
class CoinCounter :
	public HudSubPanel
{
private:

	int units;
	int dozens;
	std::vector<Number*> panel_numbers;
	


public:

	CoinCounter();
	 void Update(float hud_x,float hud_y);
	 void Render();
	virtual void reset();

};

