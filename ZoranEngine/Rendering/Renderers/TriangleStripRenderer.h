#pragma once
#include <Rendering/RenderedObjectBase.h>
#include <Rendering/Primitives.h>

class ZoranEngine_EXPORT TriangleStripRenderer : public RenderedObjectBase
{
public:
	TriangleStripRenderer(DrawType dt);
	virtual ~TriangleStripRenderer();

	virtual void MakeFullScreenQuad() = 0;
	virtual void AddRectanglesToStrip(const std::vector<RectanglePrimitive>& rectangles) = 0;
	virtual void BeginTriangles(Vector3D ll, Vector3D tc, Vector3D lr) = 0;
	virtual void AddTriangleToStrip(Vector3D nextVertex) = 0;
	virtual void EndTriangles() = 0;
};

