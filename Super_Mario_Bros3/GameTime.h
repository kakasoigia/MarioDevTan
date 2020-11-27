#pragma once
#include "HudSubPanels.h"
#include "Number.h"
class GameTime :
	public HudSubPanel
{
private:

	int units;
	int dozens;
	int hundreds;

	void resetValues();
	std::vector<Number*> panel_numbers;
public:

	GameTime();
	void Update();
	void Render();
	virtual void reset();
};

