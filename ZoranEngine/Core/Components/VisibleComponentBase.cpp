#include "stdafx.h"
#include "VisibleComponentBase.h"
#include <Rendering/RenderEngineBase.h>
#include <Rendering/RenderedObjectBase.h>

VisibleComponentBase::VisibleComponentBase(RenderEngineBase* engine) : program(0), renderedObject(0)
{
	if (engine)this->engine = engine;
	else this->engine = rEngine;
}

VisibleComponentBase::~VisibleComponentBase()
{
	if(renderedObject)
		delete renderedObject;
}