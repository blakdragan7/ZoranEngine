#include "stdafx.h"
#include "Statistics.h"

#include <string>
#include <assert.h>

#include <iomanip>
#include <sstream>

#include <ZGI/Windows/ZGIDebugWindow.h>
#include <ZGI/Panels/ZGITreePanel.h>

typedef std::pair<std::string, BenchStatChain> StatMapPair;

BenchMarker* BenchMarker::singleton = 0;

BenchMarker::BenchMarker()
{
	ShowGraph = false;
}


BenchMarker::~BenchMarker()
{
}


float BenchMarker::GetOneOverTotalSeconds()
{
	return 1.0f / (float)(rootChain.nanoseconds / NANOSECONDS_PER_SECONDS);
}

void BenchMarker::DebugDraw()
{
	return;
	DebugWindow->SetFPS(1.0f / ((double)rootChain.nanoseconds / (double)NANOSECONDS_PER_SECONDS));

	ZGITreePanel* tree = DebugWindow->GetTree();

	TreeSocket& socket = tree->GetRootSocket();

	std::stringstream ss;

	ss.str("");
	ss.clear();
	ss << "Frame Time " << ": " << std::fixed << std::setprecision(2) << (rootChain.nanoseconds / NANOSECONDS_PER_MILISECONDS)  << " ms";
	socket.SetText(ss.str());

	for (auto const& x : rootChain.map)
	{
		TreeSocket& subSocket = socket.TreeSocketNamed(x.first);
		ss.str("");
		ss.clear();
		ss << x.first << ": " << std::fixed << std::setprecision(2) << (((double)x.second.nanoseconds / (double)rootChain.nanoseconds) * 100.0);
		subSocket.SetText(ss.str());
		if (socket.IsOpen())
		{
			x.second.DebugDraw(ss, subSocket, rootChain.nanoseconds);
		}
	}
}

void  BenchMarker::AddStat(std::initializer_list<std::string> keys, long long nanoseconds)
{
	std::list<std::string> keyList(keys);
	rootChain.AddStat(keyList,nanoseconds);
}

void  BenchMarker::StartStatWithDepth(std::initializer_list<std::string> keys)
{
	std::list<std::string> keyList(keys);
	rootChain.StartClock(keyList);
}

void  BenchMarker::TakeStatWithDepth(std::initializer_list<std::string> keys)
{
	std::list<std::string> keyList(keys);
	rootChain.TakeClock(keyList);
}

void BenchMarker::AccumStatWithDepth(std::initializer_list<std::string> keys)
{
	std::list<std::string> keyList(keys);
	rootChain.AccumClock(keyList);
}

void BenchMarker::StartBench()
{
	rootChain.StartClock();
}

std::string BenchMarker::serialze()const
{
	std::string total;

	total += "FPS " + std::to_string(1.0 / (double)rootChain.nanoseconds) + "\n";
	
	for (auto const& x : rootChain.map)
	{
		total += x.first + ":" + std::to_string(x.second.nanoseconds)  + "\n";
		total += x.second.serizlize("");
	}

	return total;
}

std::ostream& operator<<(std::ostream& os, const BenchMarker& b)
{
	os << b.serialze();
	return os;
}

std::string BenchStatChain::serizlize(std::string tabs)const
{
	std::string total;

	tabs += "\n";

	for (auto const& x : map)
	{
		total += tabs + x.first + ":" + std::to_string(x.second.nanoseconds) + "\n";
		total += tabs + x.second.serizlize(tabs);
	}

	return total;
}

void BenchStatChain::DebugDraw(std::stringstream& ss, TreeSocket& socket,const long long& totalNanoseconds) const
{
	for (auto const& x : map)
	{
		TreeSocket& subSocket = socket.TreeSocketNamed(x.first);
		ss.str("");
		ss.clear();
		ss << x.first << ": " << std::fixed << std::setprecision(2) << (((double)x.second.nanoseconds / (double)totalNanoseconds) * 100.0);
		subSocket.SetText(ss.str());
		if (socket.IsOpen())
		{
			x.second.DebugDraw(ss, subSocket,totalNanoseconds);
		}
	}
}

void BenchStatChain::AddStat(std::list<std::string> keyList, const long long & nanoseconds)
{
	if (keyList.size() == 0)
	{
		this->nanoseconds = nanoseconds;
		return;
	}

	const std::string firstKey = keyList.front();
	keyList.pop_front();

	auto& iter = map.find(firstKey);
	if (iter != map.end())
	{
		iter->second.AddStat(keyList,nanoseconds);
	}
	else
	{
		BenchStatChain stat;
		stat.AddStat(keyList, nanoseconds);
		map.insert(StatMapPair(firstKey, stat));
	}
}

void BenchStatChain::StartClock(std::list<std::string> keyList)
{
	if (keyList.size() == 0)
	{
		StartClock();
		return;
	}

	const std::string firstKey = keyList.front();
	keyList.pop_front();

	auto& iter = map.find(firstKey);
	if (iter != map.end())
	{
		iter->second.StartClock(keyList);
	}
	else
	{
		BenchStatChain stat;
		stat.StartClock(keyList);
		map.insert(StatMapPair(firstKey, stat));
	}
}

void BenchStatChain::TakeClock(std::list<std::string> keyList)
{
	if (keyList.size() == 0)
	{
		TakeClock();
		return;
	}

	const std::string firstKey = keyList.front();
	keyList.pop_front();

	auto& iter = map.find(firstKey);
	if (iter != map.end())
	{
		iter->second.TakeClock(keyList);
	}
	else
	{
		BenchStatChain stat;
		stat.TakeClock(keyList);
		map.insert(StatMapPair(firstKey, stat));
	}
}

void BenchStatChain::AccumClock(std::list<std::string> keyList)
{
	if (keyList.size() == 0)
	{
		AccumClock();
		return;
	}

	const std::string firstKey = keyList.front();
	keyList.pop_front();

	auto& iter = map.find(firstKey);
	if (iter != map.end())
	{
		iter->second.AccumClock(keyList);
	}
	else
	{
		BenchStatChain stat;
		stat.AccumClock(keyList);
		map.insert(StatMapPair(firstKey, stat));
	}
}
