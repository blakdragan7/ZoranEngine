#pragma once
#include <unordered_map>
#include <vector>
#include <Utils/HighPrecisionClock.h>
#include <iostream>
#include <string>

class TreeSocket;
class ZGITreePanel;
struct BenchStatChain
{
	HighPrecisionClock clock;
	long long nanoseconds;
	std::unordered_map<std::string,BenchStatChain> map;

	BenchStatChain() :nanoseconds(0) {}
	BenchStatChain(long long nanoseconds) : nanoseconds(nanoseconds) {}

	inline void StartClock() { clock.TakeClock(); nanoseconds = 0; }
	inline void TakeClock() { nanoseconds = clock.GetDiffNanoSeconds(); }
	inline void AccumClock() { nanoseconds += clock.GetDiffNanoSeconds(); }

	std::string serizlize(std::string tabs)const;
	void DebugDraw(std::stringstream& ss, TreeSocket& socket, const long long& totalNanoseconds)const;
	void AddStat(std::list<std::string> keyList,const long long& nanoseconds);
	void StartClock(std::list<std::string> keyList);
	void TakeClock(std::list<std::string> keyList);
	void AccumClock(std::list<std::string> keyList);
};

class BenchMarker
{
private:
	static BenchMarker* singleton;
	BenchStatChain rootChain;

	// Drawing Stuff
	bool ShowGraph;

public:
	BenchMarker();
	~BenchMarker();

	void AddStat(std::initializer_list<std::string> keys,long long nanoseconds);
	void StartStatWithDepth(std::initializer_list<std::string> keys);
	void TakeStatWithDepth(std::initializer_list<std::string> keys);
	void AccumStatWithDepth(std::initializer_list<std::string> keys);

	void StartBench();
	inline void TakeBench() { rootChain.TakeClock(); }

	inline long long GetTotalNanoSeconds() { return rootChain.nanoseconds; }
	float GetOneOverTotalSeconds();

	void DebugDraw(ZGITreePanel* tree);

	inline static BenchMarker* Singleton()
	{
		if (singleton == 0)singleton = new BenchMarker();
		return singleton;
	}

	std::string serialze()const;
};

extern std::ostream& operator<<(std::ostream& os, const BenchMarker& b);

// only do bench marking if we specifiy we want to

#if defined(_BENCHMARKING)
#define DEBUG_BENCH_START BenchMarker::Singleton()->StartBench();
#define DEBUG_TAKE_BENCH BenchMarker::Singleton()->TakeBench();
#define DEBUG_ADD_STAT(a,...) BenchMarker::Singleton()->AddStat({__VA_ARGS__},a);
#define DEBUG_BENCH_START_TRACK(...) BenchMarker::Singleton()->StartStatWithDepth({__VA_ARGS__});
#define DEBUG_TRACK_TAKE_BENCH(...) BenchMarker::Singleton()->TakeStatWithDepth({__VA_ARGS__});
#define DEBUG_TRACK_ACCUM_STAT(...) BenchMarker::Singleton()->AccumStatWithDepth({__VA_ARGS__});
#define DEBUG_DRAW(tree) BenchMarker::Singleton()->DebugDraw(tree);
#elif defined(_FPSBENCHONLY)
#define DEBUG_BENCH_START BenchMarker::Singleton()->StartBench();
#define DEBUG_TAKE_BENCH BenchMarker::Singleton()->TakeBench();
#define DEBUG_ADD_STAT(a,...)
#define DEBUG_BENCH_START_TRACK(...)
#define DEBUG_TRACK_TAKE_BENCH(...)
#define DEBUG_TRACK_ACCUM_STAT(...)
#define DEBUG_DRAW BenchMarker::Singleton()->DebugDraw();
#else
#define DEBUG_BENCH_START 
#define DEBUG_TAKE_BENCH 
#define DEBUG_ADD_STAT(a,...)
#define DEBUG_BENCH_START_TRACK(...)
#define DEBUG_TRACK_TAKE_BENCH(...)
#define DEBUG_TRACK_ACCUM_STAT(...)
#define DEBUG_DRAW
#endif	
