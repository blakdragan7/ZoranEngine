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

bool GetFilesInDir(std::string inDir, std::string fileMask, bool recurse, std::function<void(std::string, std::string)> callback)
{
#ifdef _WIN32 // is windows

	HANDLE hFind;
	WIN32_FIND_DATA data;

	std::string path = inDir + "/" + fileMask;

	hFind = FindFirstFile(path.c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (strcmp(data.cFileName, ".") == 0)continue;
			else if (strcmp(data.cFileName, "..") == 0)continue;

			else if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (recurse)
				{
					GetFilesInDir(inDir + "/" + data.cFileName, fileMask, recurse, callback);
				}
				else continue;
			}
			else
			{
				callback(data.cFileName, inDir + "/" + data.cFileName);
			}
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}

#else // is not windows

#endif // end if Windows


	return true;
}
