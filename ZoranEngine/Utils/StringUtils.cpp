#include "stdafx.h"
#include "StringUtils.h"

#include <sstream>
#include <iterator>
#include <algorithm>

class RemoveStrObj
{
public:
	const std::string* toRemove;
	RemoveStrObj(const std::string* toRemove) : toRemove(toRemove) {}
	bool operator()(char c)
	{
		return toRemove->find(c) != std::string::npos;
	}
};

void FlattenString(std::string & string, const std::string toRemove)
{
	for (char c : toRemove)
	{
		string.erase(remove_if(string.begin(), string.end(), RemoveStrObj(&toRemove)), string.end());
	}
}

void RemoveStringFromString(std::string & string, const std::string toRemove)
{
	size_t size = toRemove.size();
	
	char* buff = new char[size+1];
	memset(buff, 0, size+1);

	for (size_t i = 0; i < string.size(); )
	{
		char c = string[i];

		for (size_t l = 1; l < size ; ++l)
		{
			buff[l - 1] = buff[l];
		}

		buff[size - 1] = c;

		if (strcmp(buff, toRemove.c_str()) == 0)
			string.erase(string.begin() + i - size, string.begin() + i);
		else
			++i;
	}

	delete buff;
}

size_t FindAny(const std::string & string, const std::string toFind)
{
	for (size_t i = 0; i < string.size(); i++)
	{
		char c = string[i];
		if (toFind.find(c) != std::string::npos)
		{
			return i;
		}
	}

	return std::string::npos;
}

std::vector<std::string> SplitString(const std::string & string,const std::string delims)
{
	std::vector<std::string> set;
	
	std::string currentS;
	for (char c : string)
	{
		if (delims.find(c) != std::string::npos)
		{
			set.push_back(currentS);
			currentS.clear();
		}
		else
		{
			currentS.append(1, c);
		}
	}
	set.push_back(currentS);
	return set;
}

bool StringEndsWithString(std::string const &fullString, std::string const &ending) {
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else {
		return false;
	}
}

std::string StringToLower(const std::string & source)
{
	std::string lowerString;
	lowerString.resize(source.size());
	std::transform(source.begin(), source.end(), lowerString.begin(), ::tolower);

	return lowerString;
}

std::string GetStringFileType(const std::string & filePath)
{
	std::vector<std::string> split = SplitString(filePath, ".");

	if (split.size() != 2)
	{
		return std::string();
	}

	return split[1];
}