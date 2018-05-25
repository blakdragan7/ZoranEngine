#pragma once
#include "PlatformTypes.h"

class DRAGENGINE_EXPORT HighPrecisionClock
{
private:
	long long time;

public:
	HighPrecisionClock();
	~HighPrecisionClock();

	void TakeClock();

	long long GetDiffNanoSeconds();
	double GetDiffSeconds();
};

