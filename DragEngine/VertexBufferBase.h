#pragma once
#include "PlatformTypes.h"

/*
* This class represents an abstraction to Vertex Buffers and includes every aspect of sending vertex information to the gpu
* for opengl this means it includes vertex buffers AND vertex attributes.
*/
// may have more options later but currently only has float

enum VertexType
{
	VT_Float
};

enum DrawType
{
	DT_Dynamic,
	DT_Static
};

class DRAGENGINE_EXPORT VertexBufferBase
{
protected:
	unsigned	numVerts;
	VertexType	vertType;
	DrawType	drawType;

public:
	VertexBufferBase();
	virtual ~VertexBufferBase();

	virtual void CreateBufferFromMemory(VertexType vertType, DrawType drawType,unsigned numVerts, void* verts) = 0;
	virtual void BindBuffer() = 0;
	virtual void UnbindBuffer() = 0;
	virtual void DrawArrays() = 0;
};

