#include "pch.h"
#include "ZClass.h"


ZClass::ZClass() : isInterface(false), isStruct(false)
{
}


ZClass::~ZClass()
{
}

std::string ZClass::Stringify() const
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
