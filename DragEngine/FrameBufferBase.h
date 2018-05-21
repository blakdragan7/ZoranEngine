#pragma once
#include "PlatformTypes.h"

/*
* This represents an abstraction to off screen rendering. You must subclass this class for custom behavior
*/

class DRAGENGINE_EXPORT FrameBufferBase
{
public:
	FrameBufferBase();
	virtual ~FrameBufferBase();
};

