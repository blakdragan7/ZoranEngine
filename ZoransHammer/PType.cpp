#include "pch.h"
#include "PType.h"


PType::PType() : isStatic(false)
{
}

PType::~PType()
{
}

std::string PType::Stringify() const
{
	if(typeName.empty() == false || memberName.empty() == false)
		return typeName + " " + memberName;
	return "";
}
