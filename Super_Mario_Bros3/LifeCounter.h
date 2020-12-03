#pragma once
#include "HudSubPanels.h"
#include "Number.h"
class LifeCounter :
	public HudSubPanel

{
	std::vector<Number*> panel_numbers;
	int units ;
	int dozens ;
public:
	LifeCounter();
	virtual void Update(float x, float y);
	virtual void Render();
	virtual void reset();
};

