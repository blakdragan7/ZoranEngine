#pragma once
#include "Core/PlatformTypes.h"

/*
* This represents an abstraction to off screen rendering. You must subclass this class for custom behavior
*/

class ZoranEngine_EXPORT FrameBufferBase
{
public:
	FrameBufferBase();
	virtual ~FrameBufferBase();
};

