#pragma once
#include <Rendering/RenderedObjectBase.h>

class ZoranEngine_EXPORT TriangleStripRenderer : public RenderedObjectBase
{
public:
	TriangleStripRenderer(DrawType dt);
	virtual ~TriangleStripRenderer();

	virtual void MakeFullScreenQuad() = 0;
	virtual void BeginTriangles(Vector3D ll, Vector3D tc, Vector3D lr) = 0;
	virtual void AddTriangleToStrip(Vector3D nextVertex) = 0;
	virtual void EndTriangles() = 0;
};

