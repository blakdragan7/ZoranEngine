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
	return typeName + ":" + memberName;
}
