#include "stdafx.h"
#include "VisibleComponentBase.h"
#include <Rendering/RenderEngineBase.h>
#include <Rendering/RenderedObjectBase.h>

VisibleComponentBase::VisibleComponentBase(unsigned renderLayer,RenderEngineBase* engine) : program(0) , renderLayer(renderLayer)
{
	if (engine)this->engine = engine;
	else this->engine = rEngine;

	renderedObject = this->engine->CreateRenderedObject();
}

VisibleComponentBase::~VisibleComponentBase()
{
	this->engine->RemoveComponent(this);
	delete renderedObject;
}

void VisibleComponentBase::SetShaderProgram(ShaderProgramBase * program)
{
	if (this->program)
	{
		this->engine->RemoveComponent(this);
		delete this->program;
	}
	this->program = program;
	this->engine->AddComponent(this);
}

void VisibleComponentBase::SetRenderLayer(unsigned renderLayer)
{
	//TODO: make this more efficient
	if (this->program)
	{
		this->engine->RemoveComponent(this);
		this->engine->AddComponent(this);
	}
}
