#pragma once

#include <string>
#include <vector>

class ZClass
{
public:
	std::string ClassName;
	std::vector<std::string> Parents;

	ZClass(std::string ClassName, std::vector<std::string> Parents) : ClassName(ClassName), Parents(Parents) {}

	virtual void* SpawnDynamic()const = 0;

	bool IsSubclassOf(std::string parent)const
	{
		return std::find(Parents.begin(), Parents.end(), parent) != Parents.end();
	}

	template<typename BaseClass>
	static BaseClass* SpawnClass(const ZClass* derived)
	{
		return (BaseClass*)derived->SpawnDynamic();
	}

	bool operator ==(const std::string& name)
	{
		return ClassName == name;
	}
};
