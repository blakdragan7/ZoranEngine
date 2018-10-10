#pragma once
#include <Rendering/RenderedObjectBase.h>
#include <Rendering/Primitives.h>

class ZoranEngine_EXPORT TriangleRenderer : RenderedObjectBase
{
public:
	TriangleRenderer(DrawType dt);
	virtual ~TriangleRenderer();

	virtual void MakeFullScreenQuad() = 0;
	virtual void AddTriangles(const std::vector<TrianglePrimitive>& rectangles) = 0;
};

