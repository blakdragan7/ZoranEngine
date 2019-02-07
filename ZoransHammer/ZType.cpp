#include "pch.h"
#include "ZType.h"


ZType::ZType() : isStatic(false)
{
}

ZType::~ZType()
{
}

std::string ZType::Stringify() const
{
	if(typeName.empty() == false || memberName.empty() == false)
		return typeName + " " + memberName;
	return "";
}
