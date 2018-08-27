#include "stdafx.h"
#include "VisibleComponentBase.h"
#include <Rendering/RenderEngineBase.h>

VisibleComponentBase::VisibleComponentBase(RenderEngineBase* engine)
{
	if (engine)this->engine = engine;
	else engine = rEngine;

	renderedObject = engine->CreateRenderedObject();
}

VisibleComponentBase::~VisibleComponentBase()
{
	delete renderedObject;
}
