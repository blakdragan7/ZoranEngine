#pragma once
#include "Core/PlatformTypes.h"

enum VertexType
{
	VT_Float
};

enum DrawType
{
	DT_Dynamic,
	DT_Static
};

enum PrimitiveType
{
	PT_Square,
	PT_Triangle_Strip,
	PT_Triangle,
	PT_Line_Loop,
	PT_Lines,
	PT_Dot,

};

class RenderEngineBase;
class ZoranEngine_EXPORT RenderedObjectBase
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

	virtual void CreateObjectFromMemory(PrimitiveType pType, VertexType vertType, DrawType drawType, unsigned numVerts, void* verts, void* uv, bool copy = true) = 0;
	virtual void RenderObject() = 0;
	virtual bool GetVertDataAsfloat(float** data, unsigned &amount) = 0;
	virtual bool GetVertEdgeIndexes(unsigned** indexes, unsigned &amount) = 0;
	virtual void MakeFullScreenQuad() = 0;
};

