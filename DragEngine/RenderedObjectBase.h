#pragma once
#include "PlatformTypes.h"

enum VertexType
{
	VT_Float
};

enum DrawType
{
	DT_Dynamic,
	DT_Static
};

class RenderEngineBase;
class DRAGENGINE_EXPORT RenderedObjectBase
{
protected:
	unsigned	numVerts;
	
	void*		cpuVertData;
	void*		cpuUVData;

	VertexType	vertType;
	DrawType	drawType;

	RenderEngineBase* renderEngine;

public:
	RenderedObjectBase();
	virtual ~RenderedObjectBase();

	virtual void CreateObjectFromMemory(VertexType vertType, DrawType drawType, unsigned numVerts, void* verts, void* uv, bool copy = true) = 0;
	virtual void RenderObject() = 0;
};

