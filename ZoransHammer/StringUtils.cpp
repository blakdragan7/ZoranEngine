#include "pch.h"
#include "StringUtils.h"

#include <algorithm>

class RemoveStrObj
{
public:
	std::string* toRemove;
	RemoveStrObj(std::string* toRemove) : toRemove(toRemove) {}
	bool operator()(char c)
	{
		return toRemove->find(c) != std::string::npos;
	}
};

void FlattenString(std::string & string, std::string toRemove)
{
	for (char c : toRemove)
	{
		string.erase(remove_if(string.begin(), string.end(), RemoveStrObj(&toRemove)), string.end());
	}
}

size_t FindAny(std::string & string, std::string toFind)
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

std::vector<std::string> Split(std::string & string, std::string delims)
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
