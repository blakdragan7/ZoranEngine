#pragma once

#include "ZType.h"

#include <string>
#include <vector>
struct ZFunction
{
	// pointer to return type, NULL means return type is void
	ZType returnType;
	// array of pointers to types that represent the parameters
	std::vector<ZType> parameters;
	// the name of this function
	std::string name;
	// if this is a pure virtual function (i.e. virtual fType function(...)=0)
	bool isAbstract;
	// if this function is virtual (this will be false if it's a pure virtual function)
	bool isVirtual;
	// wether this is a class or member function
	bool isStatic;
	ZFunction();
	~ZFunction();

	std::string Stringify()const;
};
