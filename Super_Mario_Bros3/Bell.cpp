#include "Bell.h"
#include "Coin.h"
#include "BreakableBrick.h"
#include "Utils.h"
void CBell::Render()
{
	if (isActive == true) return;
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CBell::GetBoundingBox(float &l, float &t, float &r, float &b)
{

	if (isUp && !isActive)
	{
		
		l = x;
		t = y;
		r = x + BELL_BBOX_WIDTH;
		b = y + BELL_BBOX_HEIGHT;
	}
	else
	{
		l = t = r = b = 0;
	}
	
}
void CBell::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	
	CGameObject::Update(dt);
	if (isActive)
	{
		
		for (UINT i = 0; i < coObjects->size(); i++)
		{
		
			LPGAMEOBJECT obj = coObjects->at(i);
				if (dynamic_cast<CBreakableBrick*>(obj))
				{
					DebugOut(L"[INFO] Vô đây r \n");
					CBreakableBrick* breakableBrick = dynamic_cast<CBreakableBrick*>(obj);
					if (breakableBrick->GetState() == BREAKABLE_STATE_SHOW)
					{
						breakableBrick->SetState(BREAKABLE_STATE_COIN);
					}
				}
				
		}
	}
}