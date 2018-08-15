#include "stdafx.h"
#include "Statistics.h"
#include <string>
#include <assert.h>

#include <ThirdParty/imgui/imgui.h>

typedef std::pair<std::string, BenchStatChain> StatMapPair;

BenchMarker* BenchMarker::singleton = 0;

BenchMarker::BenchMarker()
{
	ShowGraph = false;
}


BenchMarker::~BenchMarker()
{
}


void BenchMarker::ImGuiDraw()
{
	/*bool f = true;
	ImGui::ShowDemoWindow(&(f));*/

	ImGui::Begin("ZoranEngine Stats");

	ImGui::Checkbox("Shoow Graphs", &ShowGraph);

	ImGui::Text("FrameRate %f", 1.0 / ((double)rootChain.nanoseconds / (double)NANOSECONDS_PER_SECONDS));
	ImGui::Text("FrameTime %f", ((double)rootChain.nanoseconds / (double)NANOSECONDS_PER_SECONDS));
	if (ShowGraph)
		ImGui::PlotVar("FrameTime", ((float)rootChain.nanoseconds / (float)NANOSECONDS_PER_SECONDS),0,60,120);

	ImGui::Spacing();
	ImGui::Spacing();

	for (auto const& x : rootChain.map)
	{
		ImGui::Text(("\t" + x.first + ": " + std::to_string(((double)x.second.nanoseconds / (double)rootChain.nanoseconds) * 100.0)).c_str());
		if(ShowGraph)
			ImGui::PlotVar(x.first.c_str(),((float)x.second.nanoseconds / (float)rootChain.nanoseconds) * 100.0,0,100);
		if (ImGui::TreeNode(x.first.c_str()))
		{
			x.second.ImGuiDraw(ShowGraph, rootChain.nanoseconds);
			ImGui::TreePop();
		}

		ImGui::Spacing();

	}

	ImGui::End();

	ImGui::PlotVarFlushOldEntries();

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

void BenchStatChain::ImGuiDraw(const bool& showGraph,const long long& totalNanoseconds) const
{
	for (auto const& x : map)
	{
		ImGui::Text(("\t" + x.first + ": " + std::to_string(((double)x.second.nanoseconds / (double)totalNanoseconds) * 100.0)).c_str());
		if (showGraph)
			ImGui::PlotVar(x.first.c_str(), ((float)x.second.nanoseconds / (float)totalNanoseconds) * 100.0, 0, 100);
		if (ImGui::TreeNode(x.first.c_str()))
		{
			x.second.ImGuiDraw(showGraph,totalNanoseconds);
			ImGui::TreePop();
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
