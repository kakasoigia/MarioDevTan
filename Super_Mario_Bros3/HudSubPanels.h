#pragma once
#include "GameObject.h"
class HudSubPanel : public CGameObject
{
	/*virtual void Render();*/
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void Update(float x,float y)=0;
	
};


