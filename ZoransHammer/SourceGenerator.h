#pragma once
#include "PClass.h"

// This is a utility class for generating source files that doesnt need an instance
// like a function library
class SourceGenerator
{
public:
	// Generate SourceFile from class
	// this uses the sourceDir member of PClass to make a sub dir in dir 
	// (note if there is any ../'s in the sourceDir they will be removed) 
	static bool GenerateSourceToDir(const PClass& theClass, std::string headerDir);
};
