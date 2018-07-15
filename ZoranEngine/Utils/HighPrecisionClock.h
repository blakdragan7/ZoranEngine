#pragma once
#include "Core/PlatformTypes.h"

class ZoranEngine_EXPORT HighPrecisionClock
{
private:
	long long time;

public:
	HighPrecisionClock();
	~HighPrecisionClock();

	void TakeClock();

	long long GetDiffNanoSeconds();
	float GetDiffSeconds();
};

