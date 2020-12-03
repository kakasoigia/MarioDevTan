#pragma once
#include "HudSubPanels.h"
#include "HudPanels.h"
#include "Number.h"
class PowerMeter :
	public HudSubPanel
{
	std::vector<Number*> panel_numbers;
public:
	PowerMeter();
	virtual void Update(float x, float y);
	virtual void Render();
	virtual void reset();
};

