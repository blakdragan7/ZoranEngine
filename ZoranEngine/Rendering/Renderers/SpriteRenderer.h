#pragma once
#include <Rendering/RenderedObjectBase.h>

class ZoranEngine_EXPORT SpriteRenderer : public RenderedObjectBase
{

public:
	SpriteRenderer(PrimitiveType pt, DrawType dt);
	virtual ~SpriteRenderer();
};