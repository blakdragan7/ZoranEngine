#include "stdafx.h"
#include "FileUtils.h"
#include <string>
#include <algorithm>

#undef max
#define max(a,b) a>b?a:b

bool MoveToChar(std::fstream & inFile, char theChar)
{
	while (inFile.eof() == false)
	{
		char g = inFile.get();
		if (g == theChar)return true;
	}

	return false;
}

bool GetLine(std::fstream & inFile, std::string& line, std::string delims)
{
	line.clear();
	while (inFile.eof() == false)
	{
		char g = inFile.get();
		if (delims.find(g) != std::string::npos)return true;
		line.append(1,g);
	}

	return false;
}

bool GetLineVar(std::fstream & inFile, std::string & line, std::vector<std::string> delims)
{
	size_t maxStringSize = 0;
	
	for (auto& s : delims)
	{
		maxStringSize = max(maxStringSize, s.size());
	}

	line.clear();
	std::string test;

	while (inFile.eof() == false)
	{
		char g = inFile.get();
		test += g;

		for (auto& s : delims)
		{
			if (test.find(s) != std::string::npos)
			{
				line.erase(line.end() - s.size() + 1, line.end());
				return true;
			}
		}

		if (test.size() >= maxStringSize)
		{
			test.erase(test.begin());
		}

		line.append(1, g);
	}

	return false;
}
