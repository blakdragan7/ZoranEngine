#pragma once
#include <Rendering/RenderedObjectBase.h>

class ZoranEngine_EXPORT QuadRenderer : RenderedObjectBase
{
public:
	QuadRenderer(DrawType dt);
	virtual ~QuadRenderer();

	virtual void AddFullScreenQuad() = 0;
	virtual void AddQuad(Vector3D bl, Vector3D tl, Vector3D tr, Vector3D br) = 0;
};
