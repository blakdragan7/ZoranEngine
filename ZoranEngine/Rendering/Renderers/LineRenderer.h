#pragma once
#include <Rendering/RenderedObjectBase.h>

class ZoranEngine_EXPORT LineRenderer : RenderedObjectBase
{
public:
	LineRenderer(DrawType dt);
	virtual ~LineRenderer();

	virtual void SetLines(Vector3D lines[]) = 0;
	virtual void SetLines(Vector2D lines[]) = 0;
	virtual void AppendLines(Vector3D lines[]) = 0;
	virtual void AppendLines(Vector2D lines[]) = 0;
	virtual void AddLine(Vector2D start, Vector2D end) = 0;
	virtual void AddLine(Vector3D start, Vector3D end) = 0;
};

