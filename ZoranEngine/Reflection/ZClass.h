#pragma once

#include <string>
#include <vector>

class ZClass
{
public:
	std::string ClassName;
	std::vector<std::string> Parents;

	ZClass(std::string ClassName, std::vector<std::string> Parents);

	// default returns 0, therfore anything useing this must check the result is not 0
	virtual void* SpawnDynamic()const;

	bool IsSubclassOf(std::string parent)const;

	bool operator ==(const std::string& name);
};
