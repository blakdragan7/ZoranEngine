#pragma once
#include "Core/PlatformTypes.h"

class ZoranEngine_EXPORT Random
{
public:
	static void Init();
	static float GetFloatInRange(float low, float hi);
	static double GetDoubleInRange(double low, double hi);
	static int GetIntInRange(int low, int hi);
	static long GetLongInRange(long low, long hi);
	static unsigned GetUnsignedInRange(unsigned low, unsigned hi);
};

