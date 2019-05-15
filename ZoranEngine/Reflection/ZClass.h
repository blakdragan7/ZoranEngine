#pragma once

#include <string>
#include <vector>

class ZoranEngine_EXPORT ZClass
{
public:
	std::string ClassName;
	std::vector<std::string> Parents;

	ZClass(std::string ClassName, std::vector<std::string> Parents);

	// default returns nullptr, therfore anything useing this must check the result is not nullptr
	virtual void* SpawnDynamic()const;

	bool IsSubclassOf(std::string parent)const;

	bool operator ==(const std::string& name);
};
