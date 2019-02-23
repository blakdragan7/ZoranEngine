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
	int currentLine;

private:
	void RemoveEmptyStrings(std::vector<std::string>& vector);

	std::vector<std::string> SplitAtFirst(std::string& string, const char delim);

	bool SkipTemplate(std::fstream& inFile);

	bool MoveOutOfImplementation(std::fstream& inFile, int& depth);
	// return 0 if coudln't find end of class dec, return 1 if found and it's an implementation and 2 if it's a decleration only
	int GoToEndOfClassDec(std::fstream& inFile, std::string& line);

	ZType ParseType(std::string& info);
	bool ParseFunction(std::string& info, ZFunction& function);
	int ParseClassDeclaration(ZClass& theClass, std::string info);

	// ignores comments (/* */ and //) and gets everthing between ':\',':\r',';' and ': '
	bool GetLine(std::fstream& inFile, std::string& line);

public:
	SourceParser();
	~SourceParser();

	bool ParseFile(std::string& file, std::string& dir);

	/* possibly change to a move if copying zclass becomes expensive or the zclass destructor becomes expensive*/
	inline std::vector<ZClass> GetGeneratedClass() { return generated; }
};

