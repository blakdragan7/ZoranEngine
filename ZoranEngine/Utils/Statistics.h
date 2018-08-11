#pragma once
#include <unordered_map>
#include <vector>
#include <Utils/HighPrecisionClock.h>
#include <iostream>
#include <string>

struct LowLevelStat
{
	HighPrecisionClock clock;
	long long nanoseconds;

	LowLevelStat() :nanoseconds(0) {}
	LowLevelStat(long long nanoseconds) : nanoseconds(nanoseconds) {}

	inline void StartClock() { clock.TakeClock(); }
	inline void TakeClock() { nanoseconds = clock.GetDiffNanoSeconds(); }
};

struct TopLevelStat
{
	HighPrecisionClock clock;
	long long rootLevelClock;
	std::unordered_map<std::string, LowLevelStat> stats;

	TopLevelStat() : rootLevelClock(0) {}
	TopLevelStat(long long rootLevelClock) : rootLevelClock(rootLevelClock) {}

	inline void StartClock() { clock.TakeClock(); }
	inline void TakeClock() { rootLevelClock = clock.GetDiffNanoSeconds(); }
};

class BenchMarker
{
private:
	static BenchMarker* singleton;
	std::unordered_map<std::string, TopLevelStat> statsMap;

	long long totalNanoseconds;
	HighPrecisionClock clock;

	// Drawing Stuff
	bool ShowGraph;

public:
	BenchMarker();
	~BenchMarker();

	void AddHighLevelStat(std::string key,long long nanoseconds);
	void AddStat(std::string key,std::string statDescription,long long nanoseconds);
	
	void StartStat(std::string key, std::string statDescription);
	void TakeStat(std::string key, std::string statDescription);

	void StartStat(std::string key);
	void TakeStat(std::string key);

	bool GetLowLevelStatFor(const std::string & key, const std::string& statDescription, LowLevelStat** stat);
	bool GetTopLevelStatFor(const std::string & key, TopLevelStat** stat);

	inline void StartBench() { clock.TakeClock(); }
	inline void TakeBench() { totalNanoseconds = clock.GetDiffNanoSeconds(); }

	inline long long GetTotalNanoSeconds() { return totalNanoseconds; }

	void ImGuiDraw();

	inline static BenchMarker* Singleton()
	{
		if (singleton == 0)singleton = new BenchMarker();
		return singleton;
	}

	std::string serialze()const;
};

extern std::ostream& operator<<(std::ostream& os, const BenchMarker& b);

// only do bench marking if we specifiy we want to

#if (defined(_DEBUG) || defined(_RELEASE_BENCH)) && defined(_BENCHMARKING)
#define DEBUG_BENCH_START BenchMarker::Singleton()->StartBench();
#define DEBUG_TAKE_BENCH BenchMarker::Singleton()->TakeBench();
#define DEBUG_BENCH_TOP_START(a) BenchMarker::Singleton()->StartStat(a);
#define DEBUG_TAKE_TOP_BENCH(a) BenchMarker::Singleton()->TakeStat(a);
#define DEBUG_BENCH_LOW_START(a,b) BenchMarker::Singleton()->StartStat(a,b);
#define DEBUG_TAKE_LOW_BENCH(a,b) BenchMarker::Singleton()->TakeStat(a,b);
#define DEBUG_ADD_STAT(a,b,c) BenchMarker::Singleton()->AddStat(a,b,c);
#define DEBUG_DRAW BenchMarker::Singleton()->ImGuiDraw();
#else
#define DEBUG_BENCH_START
#define DEBUG_TAKE_BENCH
#define DEBUG_BENCH_TOP_START(a)
#define DEBUG_TAKE_TOP_BENCH(a)
#define DEBUG_BENCH_START(a, b)
#define DEBUG_TAKE_BENCH(a, b)
#define DEBUG_ADD_STAT(a, b, c)
#define DEBUG_DRAW
#endif	
