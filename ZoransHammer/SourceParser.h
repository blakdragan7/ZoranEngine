#pragma once
#include <string>
#include <vector>

#include "ZClass.h"
#include "ZType.h"
#include "ZFunction.h"

class SourceParser
{
private:
	std::vector<ZClass> generated;

private:
	ZType ParseType(std::string& info);
	ZFunction ParseFunction(std::string& info);

	// ignores comments (/* */ and //) and gets everthing between ':\',':\r',';' and ': '
	bool GetLine(std::fstream& inFile, std::string& line);

public:
	SourceParser();
	~SourceParser();

	bool ParseFile(std::string file);

	/* possibly change to a move if copying zclass becomes expensive or the zclass destructor becomes expensive*/
	inline std::vector<ZClass> GetGeneratedClass() { return generated; }
};

