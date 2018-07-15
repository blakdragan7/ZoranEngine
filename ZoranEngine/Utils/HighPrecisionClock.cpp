#include "stdafx.h"
#include "HighPrecisionClock.h"
#include <chrono>

using namespace std::chrono;

#define NANOSECONDS_PER_SECONDS 1e+9

HighPrecisionClock::HighPrecisionClock()
{
	TakeClock();
}

HighPrecisionClock::~HighPrecisionClock()
{
}

void HighPrecisionClock::TakeClock()
{
	time = high_resolution_clock::now().time_since_epoch().count();
}

long long HighPrecisionClock::GetDiffNanoSeconds()
{
	return (high_resolution_clock::now().time_since_epoch().count() - time);
}

float HighPrecisionClock::GetDiffSeconds()
{
	return (float)GetDiffNanoSeconds() / (float)NANOSECONDS_PER_SECONDS;
}
