#include "stdafx.h"
#include "VisibleComponentBase.h"
#include <Rendering/RenderEngineBase.h>
#include <Rendering/RenderedObjectBase.h>

VisibleComponentBase::VisibleComponentBase(RenderEngineBase* engine) : program(0)
{
	if (engine)this->engine = engine;
	else this->engine = rEngine;

	renderedObject = this->engine->CreateRenderedObject();
}

VisibleComponentBase::~VisibleComponentBase()
{
	delete renderedObject;
}