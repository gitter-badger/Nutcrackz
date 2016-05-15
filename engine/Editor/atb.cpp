#include "atb.h"
#include "lightscommon.h"

TwType TW_TYPE_OGLDEV_VECTOR3F;
TwType TW_TYPE_OGLDEV_ATTENUATION;

ATB::ATB()
{

}

bool ATB::Init()
{
	bool ret = false;

	if (TwInit(TW_OPENGL_CORE, NULL) == 1)
	{
		TwStructMember Vector3fMembers[] = {
			{ "x", TW_TYPE_FLOAT, offsetof(Vector3f, x), "" },
			{ "y", TW_TYPE_FLOAT, offsetof(Vector3f, y), "" },
			{ "z", TW_TYPE_FLOAT, offsetof(Vector3f, z), "" }
		};

		TW_TYPE_OGLDEV_VECTOR3F = TwDefineStruct("Vector3f", Vector3fMembers, 3, sizeof(Vector3f), NULL, NULL);

		TwStructMember AttenuationMembers[] = {
			{ "Const", TW_TYPE_FLOAT, offsetof(LightAttenuation, Constant), "" },
			{ "Linear", TW_TYPE_FLOAT, offsetof(LightAttenuation, Linear), "" },
			{ "Exp", TW_TYPE_FLOAT, offsetof(LightAttenuation, Exp), "" }
		};

		TW_TYPE_OGLDEV_ATTENUATION = TwDefineStruct("Attenuation", AttenuationMembers, 3, sizeof(LightAttenuation), NULL, NULL);

		ret = true;
	}

	return ret;
}

bool ATB::PassiveMouseCB(int x, int y)
{
	return (TwMouseMotion(x, y) == 1);
}