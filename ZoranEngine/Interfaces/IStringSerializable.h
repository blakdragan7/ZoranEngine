#pragma once
#include <string>

class IStringSerializable
{
public:
	virtual ~IStringSerializable() {}

	virtual std::string Serialize()const = 0;
	virtual bool FromString(const std::string&) = 0;

	virtual void operator=(const std::string& rh) { FromString(rh); }
	virtual  operator std::string()const { return Serialize(); }
};
