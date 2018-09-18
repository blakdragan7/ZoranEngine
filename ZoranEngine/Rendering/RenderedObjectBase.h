#pragma once
#include "Core/PlatformTypes.h"

/* all implementation must be added here */

#define INVALID_IMPLEMENTATION 0u
#define OPENGL_IMPLEMENTATION 1u

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
private:
	unsigned renderObjectType;

protected:
	unsigned	numVerts;
	
	void*		cpuVertData;
	void*		cpuUVData;

	VertexType	vertType;
	DrawType	drawType;

public:
	RenderedObjectBase(unsigned type = INVALID_IMPLEMENTATION);
	virtual ~RenderedObjectBase();

	virtual void RenderObject() = 0;
	virtual void MakeFullScreenQuad() = 0;
	virtual bool GetVertDataAsfloat(float** data, unsigned &amount) = 0;
	virtual void UpdateObjectFromMemory(unsigned numVerts, unsigned offset, void* verts, void* uv, bool copy = true) = 0;
	virtual void CreateObjectFromMemory(PrimitiveType pType, VertexType vertType, DrawType drawType, unsigned numVerts, void* verts, void* uv, bool copy = true) = 0;

	virtual void SetAlphaEnabled(bool enabled) = 0;

	inline unsigned GetImplementationType() { return renderObjectType; }
};
