#pragma once
#include <Rendering/RenderedObjectBase.h>
#include <Rendering/Primitives.h>

class ZoranEngine_EXPORT TriangleRenderer : RenderedObjectBase
{

public:
	TriangleRenderer(DrawType dt);
	virtual ~TriangleRenderer();

	virtual void MakeFullScreenQuad() = 0;
	virtual void AddTriangles(const std::vector<TrianglePrimitive>& rectangles) = 0; // slow but easy to use
	virtual void AddTriangles(const std::vector<float>& verts, std::vector<float>& uvs) = 0; // faster
	virtual void AddTriangles(float* verts, size_t vertSize, float* uvs, size_t uvSize) = 0;
};
