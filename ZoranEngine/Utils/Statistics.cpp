#include "stdafx.h"
#include "Statistics.h"
#include <string>
#include <assert.h>

#include <ThirdParty/imgui/imgui.h>

typedef std::pair<std::string, LowLevelStat> LowLevelPair;
typedef std::pair<std::string, TopLevelStat> TopLevelPair;

BenchMarker* BenchMarker::singleton = 0;

BenchMarker::BenchMarker()
{
	ShowGraph = false;
}


BenchMarker::~BenchMarker()
{
}

void BenchMarker::AddHighLevelStat(std::string key, long long nanoseconds)
{
	auto& iter = statsMap.find(key);
	if (iter != statsMap.end())
	{
		iter->second.rootLevelClock = nanoseconds;
	}
	else
	{
		TopLevelStat stats(nanoseconds);
		statsMap.insert(TopLevelPair(key, stats));
	}
}

void BenchMarker::AddStat(std::string key, std::string statDescription, long long nanoseconds)
{
	LowLevelStat* stat = 0;

	if (GetLowLevelStatFor(key, statDescription, &stat))
	{
		stat->nanoseconds = nanoseconds;
	}
	else
	{
		TopLevelStat* stat = 0;
		if (GetTopLevelStatFor(key, &stat))
		{
			stat->stats.insert(LowLevelPair(statDescription, LowLevelStat(nanoseconds)));
		}
		else
		{
			TopLevelStat stat;
			stat.stats.insert(LowLevelPair(statDescription, LowLevelStat(nanoseconds)));
			statsMap.insert(TopLevelPair(key, stat));
		}
	}
}

void BenchMarker::StartStat(std::string key, std::string statDescription)
{
	LowLevelStat* stat = 0;

	if (GetLowLevelStatFor(key, statDescription, &stat))
	{
		stat->StartClock();
	}

}

void BenchMarker::TakeStat(std::string key, std::string statDescription)
{
	LowLevelStat* stat = 0;

	if (GetLowLevelStatFor(key, statDescription, &stat))
	{
		stat->TakeClock();
	}
	else
	{
		TopLevelStat* stat = 0;
		if (GetTopLevelStatFor(key, &stat))
		{
			stat->stats.insert(LowLevelPair(statDescription,LowLevelStat()));
		}
		else
		{
			TopLevelStat stat;
			stat.stats.insert(LowLevelPair(statDescription, LowLevelStat()));
			statsMap.insert(TopLevelPair(key, stat));
		}
	}
}

void BenchMarker::StartStat(std::string key)
{
	TopLevelStat* stat = 0;
	if (GetTopLevelStatFor(key,&stat))
	{
		stat->StartClock();
	}
	else
	{
		statsMap.insert(TopLevelPair(key, TopLevelStat()));
	}
}

void BenchMarker::TakeStat(std::string key)
{
	TopLevelStat* stat = 0;
	if (GetTopLevelStatFor(key, &stat))
	{
		stat->TakeClock();
	}
}

bool BenchMarker::GetLowLevelStatFor(const std::string & key, const std::string & statDescription, LowLevelStat** stat)
{
	auto& iter = statsMap.find(key);
	if (iter != statsMap.end())
	{
		auto& initer = iter->second.stats.find(statDescription);
		if (initer != iter->second.stats.end())
		{
			*stat = &initer->second;
			return true;
		}
	}

	return false;

}

bool BenchMarker::GetTopLevelStatFor(const std::string & key, TopLevelStat ** stat)
{
	auto& iter = statsMap.find(key);
	if (iter != statsMap.end())
	{
		*stat = &iter->second;
		return true;
	}
	return false;
}

void BenchMarker::ImGuiDraw()
{
	/*bool f = true;
	ImGui::ShowDemoWindow(&(f));*/

	ImGui::Begin("ZoranEngine Stats");

	ImGui::Checkbox("Shoow Graphs", &ShowGraph);

	ImGui::Text("FrameRate %f", 1.0 / ((double)totalNanoseconds / (double)NANOSECONDS_PER_SECONDS));
	ImGui::Text("FrameTime %f", ((double)totalNanoseconds / (double)NANOSECONDS_PER_SECONDS));
	if (ShowGraph)
		ImGui::PlotVar("FrameTime", ((double)totalNanoseconds / (double)NANOSECONDS_PER_SECONDS),0,60,120);

	ImGui::Spacing();

	for (auto const& x : statsMap)
	{
		ImGui::Text(("\t" + x.first + ": " + std::to_string(((double)x.second.rootLevelClock / (double)totalNanoseconds) * 100.0)).c_str());
		if(ShowGraph)
			ImGui::PlotVar(x.first.c_str(),((double)x.second.rootLevelClock / (double)totalNanoseconds) * 100.0,0,1);
		if (ImGui::CollapsingHeader(x.first.c_str()))
		{
			for (auto const& s : x.second.stats)
			{
				ImGui::Text((s.first + ": " + std::to_string(((double)s.second.nanoseconds / (double)totalNanoseconds) * 100.0)).c_str());
				if (ShowGraph)
					ImGui::PlotVar(s.first.c_str(), ((double)s.second.nanoseconds / (double)totalNanoseconds) * 100.0, 0, 100);
			}
		}

		ImGui::Spacing();

	}

	ImGui::End();

	ImGui::PlotVarFlushOldEntries();

}

std::string BenchMarker::serialze()const
{
	std::string total;

	for (auto const& x : statsMap)
	{
		total += x.first + ":" + std::to_string(x.second.rootLevelClock)  + "\n";

		for (auto const& s : x.second.stats)
		{
			total += "\t" + s.first + ": " + std::to_string(s.second.nanoseconds) + "\n";
		}
	}

	return total;
}

std::ostream& operator<<(std::ostream& os, const BenchMarker& b)
{
	os << b.serialze();
	return os;
}