#pragma once
#include "HudSubPanels.h"
#include "HudPanels.h"
#include "Number.h"
#define NUMBER_ANI_0 0
#define NUMBER_ANI_1 1
#define NUMBER_ANI_2 2
#define NUMBER_ANI_3	3
#define NUMBER_ANI_4 4
#define NUMBER_ANI_5 5
#define NUMBER_ANI_6 6
#define NUMBER_ANI_7 7
#define NUMBER_ANI_8 8
#define NUMBER_ANI_9 9
class NumericalPanel : public HudSubPanel
{
private:

	

protected:

	
	vector<LPGAMEOBJECT>panel_numbers;

public:

	NumericalPanel();
	 void RenderNumber();
	virtual void doUpdate();
	virtual void reset() = 0;

};