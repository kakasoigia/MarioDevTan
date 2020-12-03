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
	virtual void Update(float x,float y);
	virtual void Render();
	virtual void reset();
};

