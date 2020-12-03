#pragma once
//#include "NumericalPanels.h"
#include "Number.h"
#include "HudSubPanels.h"
#include "Game.h"
class Score : public HudSubPanel
{

private:

	int units;
	int dozens;
	int hundreds;
	int thousands;
	int ten_thousands;
	int hundreds_thousands;
	int millions;

	std::vector<Number*> panel_numbers;
	
public:

	Score();
	virtual void Update(float x,float y);
	virtual void Render();
	virtual void reset();
};