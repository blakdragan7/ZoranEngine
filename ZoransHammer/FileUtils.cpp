#include "pch.h"
#include "FileUtils.h"
#include <string>
#include <algorithm>
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
