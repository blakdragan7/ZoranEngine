#include "pch.h"
#include "PFunction.h"


PFunction::PFunction():isAbstract(false), isVirtual(false), isStatic(false)
{
}


PFunction::~PFunction()
{
}

std::string PFunction::Stringify() const
{
	std::string s = returnType.Stringify();
	if (s.empty())
		s += name + "(";
	else
		s += " " + name + "(";
	for (auto p : parameters)
	{
		s += p.Stringify() + ",";
	}
	s += ")";
	return s;
}
