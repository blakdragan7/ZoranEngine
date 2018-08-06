#pragma once
#include "Core/PlatformTypes.h"
#include <chrono>

#define NANOSECONDS_PER_SECONDS 1e+9

class HighPrecisionClock
{
private:
	std::chrono::time_point<std::chrono::steady_clock> first;

public:
	HighPrecisionClock() { TakeClock(); }
	~HighPrecisionClock() {}

	inline void TakeClock() { first = std::chrono::steady_clock::now(); }

	inline long long GetDiffNanoSeconds() { return (std::chrono::steady_clock::now() - first).count(); }
	inline double GetDiffSeconds() {return (double)GetDiffNanoSeconds() / (double)NANOSECONDS_PER_SECONDS;}
};

