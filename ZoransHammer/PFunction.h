#pragma once

#include "PType.h"

#include <string>
#include <vector>
struct PFunction
{
	// pointer to return type, NULL means return type is void
	PType returnType;
	// array of pointers to types that represent the parameters
	std::vector<PType> parameters;
	// the name of this function
	std::string name;
	// if this is a pure virtual function (i.e. virtual fType function(...)=0)
	bool isAbstract;
	// if this function is virtual (this will be false if it's a pure virtual function)
	bool isVirtual;
	// wether this is a class or member function
	bool isStatic;
	PFunction();
	~PFunction();

	std::string Stringify()const;
};
