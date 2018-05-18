#pragma once
#include "PlatformTypes.h"

/*
* This class represents an abstraction to Vertex Buffers and includes every aspect of sending vertex information to the gpu
* for opengl this means it includes vertex buffers AND vertex attributes.
*/
class DRAGENGINE_EXPORT VertexBufferBase
{
public:
	VertexBufferBase();
	virtual ~VertexBufferBase();
};

