#pragma once

#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>

static inline bool StringEndsWithString(std::string const &fullString, std::string const &ending) {
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else {
		return false;
	}
}

static std::string StringToLower(const std::string & source)
{
	std::string lowerString;
	lowerString.resize(source.size());
	std::transform(source.begin(), source.end(), lowerString.begin(), ::tolower);

	return lowerString;
}

static std::vector<std::string> SplitString(const std::string& source, char delim)
{
	std::vector<std::string> ret;

	if (source.empty())
	{
		Log(LogLevel_Warning, "Trying To Split Empty String ! \n");
		return ret;
	}

	std::stringstream ss(source);
	std::string temp;
	while (std::getline(ss, temp, delim))
	{
		ret.push_back(temp);
	}

	return ret;
}

static std::string GetStringFileType(const std::string & filePath)
{
	std::vector<std::string> split = SplitString(filePath, '.');
	
	if (split.size() != 2)
	{
		return std::string();
	}

	return split[1];
}