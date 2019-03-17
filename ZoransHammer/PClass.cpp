#include "pch.h"
#include "PClass.h"


PClass::PClass() : isInterface(false), isStruct(false)
{
}


PClass::~PClass()
{
}

std::string PClass::Stringify() const
{
	std::string s;

	s += name;

	if (parents.size())
	{
		s += "\n superClass(es): ";
		for (auto& p : parents)
		{
			s += "\n  " + p;
		}
	}

	if (members.size())
	{
		s += "\n member(s):";

		for (auto m : members)
		{
			s += "\n  " + m.Stringify();
		}
	}

	if (functions.size())
	{
		s += "\n function(s):";

		for (auto f : functions)
		{
			s += "\n  " + f.Stringify();
		}
	}

	return s;
}
