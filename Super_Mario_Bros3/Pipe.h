#pragma once
#include "GameObject.h"
#include "define.h"

class CPipe : public CGameObject
{
	int type;
public:
	CPipe(int ctype);
	int GetType()
	{
		return type;
	}
	void SetType(int ctype)
	{
		type = ctype;
	}
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};