#pragma once
#include <Rendering/RenderedObjectBase.h>
#include <Rendering/Primitives.h>

class ZoranEngine_EXPORT IndexedTriangleRenderer : RenderedObjectBase
{
public:
	IndexedTriangleRenderer(DrawType dt);
	virtual ~IndexedTriangleRenderer();

	virtual void MakeFullScreenQuad() = 0;
	virtual void SetTriangles(const std::vector<TrianglePrimitive>& rectangles) = 0;
	virtual void SetIndecies(std::vector<unsigned>& indecies) = 0;
};
