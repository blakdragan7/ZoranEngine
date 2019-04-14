#include "stdafx.h"
#include "ZClassManager.h"

ZClassManager* ZClassManager::TheManager = 0;

ZClassManager::ZClassManager()
{
}

std::map<std::string, const ZClass*>::iterator ZClassManager::begin()
{
	return classMap.begin();
}

std::map<std::string, const ZClass*>::iterator ZClassManager::end()
{
	return classMap.end();
}

const ZClass * ZClassManager::ClassForName(const std::string & ClassName)
{
	auto iter = classMap.find(ClassName);
	if (iter != classMap.end())return iter->second;

	LOG_ERROR << "No Class Found Named " << ClassName << std::endl;
	return nullptr;
}

bool ZClassManager::RegisterClassForName(const std::string & ClassName, const ZClass * Class)
{
	if(classMap.size() > 0)
	{
		auto iter = classMap.find(ClassName);
			if (iter != classMap.end())
			{
				LOG_ERROR << "Class Named " << ClassName << "Already Exists !!" << std::endl;
					return false;
			}
	}

	classMap.insert({ClassName, Class});

	return true;
}

ZClassManager * ZClassManager::ClassManager()
{
	if (TheManager == 0)TheManager = new ZClassManager;
	return TheManager;
}
