#pragma once
#include "GameObject.h"
class Number :
	public CGameObject
{
	bool isMeterUp = true;
public:
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void SetIsMeterUp(bool set)
	{
		isMeterUp = set;
	}
	bool GetIsMeterUp()
	{
		return isMeterUp;
	}
	
};

