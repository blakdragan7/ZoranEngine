#pragma once
#include <Math/Matrix44.h>
/* Zoran GUI Interface Base */

class ZoranEngine_EXPORT ZGIBase
{
public:
	ZGIBase();
	virtual ~ZGIBase();

	/* TODO finish designing this

	virtual std::string Serialize() = 0;
	virtual bool LoadFromString(const std::string& SerializedZGI) = 0;
	*/
};

