#pragma once

#include "ZFunction.h"

#include <string>
#include <vector>

struct ZClass
{
	std::string sourceFile;
	std::string sourceDir;

	std::vector<ZType> members;
	std::vector<ZFunction> functions;
	// this is the name of the class
	std::string name;
	// parentClass, can be NULL if not a child class
	std::vector <std::string> parents;
	// determines if this class is an interface or not (parsed by guessing as all abstract functions)
	bool isInterface;
	// if a class only contains a constructor, destructor and memer variables, it is considered a struct and this will return true
	bool isStruct;
	ZClass();
	~ZClass();

	std::string Stringify()const;
};
