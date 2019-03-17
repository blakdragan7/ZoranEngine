#pragma once

#include "PFunction.h"

#include <string>
#include <vector>

struct PClass
{
	std::string sourceFile;
	std::string sourceDir;

	std::vector<PType> members;
	std::vector<PFunction> functions;
	// this is the name of the class
	std::string name;
	// parentClass, can be NULL if not a child class
	std::vector <std::string> parents;
	// determines if this class is an interface or not (parsed by containing any abstract functions)
	bool isInterface;
	// if a class only contains a constructor, destructor and memer variables, it is considered a struct and this will return true
	bool isStruct;
	PClass();
	~PClass();

	std::string Stringify()const;
};
