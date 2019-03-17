#include "pch.h"
#include "ZClass.h"

#include "ZClassManager.h"

ZClass::ZClass(std::string ClassName, std::vector<std::string> Parents) : ClassName(ClassName), Parents(Parents) 
{ 
	CM->RegisterClassForName(ClassName, this); 
}

void * ZClass::SpawnDynamic() const
{
	return nullptr;
}

bool ZClass::IsSubclassOf(std::string parent) const
{
	return std::find(Parents.begin(), Parents.end(), parent) != Parents.end();
}

bool ZClass::operator==(const std::string & name)
{
	return ClassName == name;
}
