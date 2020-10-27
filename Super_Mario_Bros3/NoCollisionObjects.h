#pragma once
#include "GameObject.h"

#define BLACKSPOT_BBOX_WIDTH  8
#define BRICK_BBOX_HEIGHT 16

class CNoCollisionObjects : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};