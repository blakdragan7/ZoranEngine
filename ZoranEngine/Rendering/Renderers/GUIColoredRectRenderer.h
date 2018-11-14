#pragma once
#include <Rendering\RenderedObjectBase.h>
#include <Math/Matrix44.h>
#include <Rendering/Primitives.h>

class TriangleStripRenderer;
class GUIColoredRectRenderer : public RenderedObjectBase
{
protected:
	Color ColorA;
	Color ColorB;

public:
	GUIColoredRectRenderer();
	~GUIColoredRectRenderer();

	inline void SetColorA(const Color& color) { ColorA = color; }
	inline void SetColorB(const Color& color) { ColorB = color; }

	virtual void SetLinearBlend() = 0;
};

