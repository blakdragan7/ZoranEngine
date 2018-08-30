#include "stdafx.h"
#include "VisibleComponentBase.h"
#include <Rendering/RenderEngineBase.h>

VisibleComponentBase::VisibleComponentBase(RenderEngineBase* engine) : program(0)
{
	if (engine)this->engine = engine;
	else engine = rEngine;

	renderedObject = engine->CreateRenderedObject();
}

VisibleComponentBase::~VisibleComponentBase()
{
	delete renderedObject;
}
