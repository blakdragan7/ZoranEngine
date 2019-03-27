#pragma once

#include <chrono>

#define NANOSECONDS_PER_SECONDS 1e+9
#define NANOSECONDS_PER_MILISECONDS 1e+6

class HighPrecisionClock
{
private:
	std::chrono::time_point<std::chrono::steady_clock> first;

public:
	HighPrecisionClock() { }
	HighPrecisionClock(const HighPrecisionClock& other) : first(other.first) { }
	~HighPrecisionClock() {}

	inline void TakeClock() { first = std::chrono::steady_clock::now(); }

	inline long long GetDiffNanoSeconds() { return (std::chrono::steady_clock::now() - first).count(); }
	inline double GetDiffSeconds() {return (double)GetDiffNanoSeconds() / (double)NANOSECONDS_PER_SECONDS;}
};

