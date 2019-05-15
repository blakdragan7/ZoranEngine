#pragma once

#include <string>
#include <map>

#include "ZClass.h"

class ZoranEngine_EXPORT ZClassManager
{
private:
	static ZClassManager* TheManager;
	std::map<const std::string, const ZClass*> classMap;

	ZClassManager();
public:

	// SpawnDynamic can return 0 so return result must be checked.
	template<typename BaseClass>
	BaseClass* SpawnClass(const ZClass* derived)
	{
		return (BaseClass*)derived->SpawnDynamic();
	}

	// SpawnDynamic can return 0 so return result must be checked.
	template<typename BaseClass>
	BaseClass* SpawnClass(const std::string name)
	{
		const ZClass* derived = ClassForName(name);
		if (derived)
		{
			return (BaseClass*)derived->SpawnDynamic();
		}

		return nullptr;
	}

	std::map<std::string, const ZClass*>::iterator begin();
	std::map<std::string, const ZClass*>::iterator end();
									 
	const ZClass* ClassForName(const std::string& ClassName);
	bool RegisterClassForName(const std::string& ClassName, const ZClass* Class);

	static ZClassManager* ClassManager();
};

#define CM ZClassManager::ClassManager()
#define SPAWN_ZCLASS(base, name) CM->SpawnClass<base>(name)