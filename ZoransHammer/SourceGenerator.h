#pragma once
#include "ZClass.h"

// This is a utility class for generating source files that doesnt need an instance
// like a function library
class SourceGenerator
{
public:
	// Generate SourceFile from class
	// this uses the sourceDir member of ZClass to make a sub dir in dir 
	// (note if there is any ../'s in the sourceDir they will be removed) 
	static bool GenerateSourceToDir(const ZClass& theClass, std::string dir);
};
