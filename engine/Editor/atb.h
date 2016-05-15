#pragma once

#include "ATB/AntTweakBar.h"

class ATB
{
public:
	ATB();

	bool Init();

	bool PassiveMouseCB(int x, int y);
};


extern TwType TW_TYPE_OGLDEV_VECTOR3F;
extern TwType TW_TYPE_OGLDEV_ATTENUATION;