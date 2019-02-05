#include "pch.h"
#include "ZFunction.h"


ZFunction::ZFunction():isAbstract(false), isVirtual(false), isStatic(false)
{
}


ZFunction::~ZFunction()
{
}

std::string ZFunction::Stringify() const
{
	std::string s = returnType.Stringify();
	s += " " + name + "(";
	for (auto p : parameters)
	{
		s += p.Stringify() + ",";
	}
	s += ")";
	return s;
}
