#include "pch.h"
#include "ZClass.h"


ZClass::ZClass() : superClass(0), isInterface(false), isStruct(false)
{
}


ZClass::~ZClass()
{
}

std::string ZClass::Stringify() const
{
	std::string s;

	s += name;

	s += "\nmembers:";

	for (auto m : members)
	{
		s += "\n\t"+ m.Stringify();
	}

	s += "\nfunctions:";

	for (auto f : functions)
	{
		s += "\n\t" + f.Stringify();
	}

	return s;
}
