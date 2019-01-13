#pragma once
#include "Core/PlatformTypes.h"

/* all implementation must be added here */
enum VertexType
{
	VT_Unused,
	VT_Float
};

enum DrawType
{
	DT_Unused,
	DT_Dynamic,
	DT_Static
};

enum PrimitiveType
{
	PT_Unused,
	PT_Quad_Strip,
	PT_Quads,
	PT_Triangle_Strip,
	PT_Triangles,
	PT_Line_Loop,
	PT_Lines,
	PT_Dots
};

class Matrix44;
class RenderEngineBase;
class ZoranEngine_EXPORT RenderedObjectBase
{
private:
	PrimitiveType primitiveType;
	VertexType	vertType;
	DrawType	drawType;

public:
	RenderedObjectBase();
	RenderedObjectBase(PrimitiveType pt, VertexType vt, DrawType dt);
	virtual ~RenderedObjectBase();

	virtual void RenderObject(const Matrix44& cameraMatrix) = 0;

	inline VertexType GetVertType()const { return vertType; }
	inline DrawType GetDrawType()const { return drawType; }
	inline PrimitiveType GetPrimitiveType()const { return primitiveType; }
};
