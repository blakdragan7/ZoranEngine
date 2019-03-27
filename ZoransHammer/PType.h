#pragma once

#include <string>

struct PType
{
	// string representation of this type (e.x. int)
	std::string typeName;
	// name of this type (ex in int A the A)
	// if this type is a return type then this will be empty
	std::string memberName;
	// wether this is a class or member variable
	bool isStatic;

	PType();
	~PType();

	std::string Stringify()const;
};