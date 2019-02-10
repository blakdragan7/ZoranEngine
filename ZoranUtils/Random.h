#pragma once
#include "PlatoformTypes.h"

class DLL_EXPORT Random
{
public:
	static void Init();
	static float GetFloatInRange(float low, float hi);
	static float GetfloatInRange(float low, float hi);
	static int GetIntInRange(int low, int hi);
	static long GetLongInRange(long low, long hi);
	static unsigned GetUnsignedInRange(unsigned low, unsigned hi);
	static bool GetBoolWithChance(float percent); // percent from 0 -> 1
};

