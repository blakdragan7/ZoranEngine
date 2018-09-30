#include "stdafx.h"
#include "Visible2DComponent.h"
#include <Rendering/RenderedObjectBase.h>
#include <Rendering/RenderEngine2DBase.h>

Visible2DComponent::Visible2DComponent(unsigned renderLayer, RenderEngine2DBase* engine) :
	renderLayer(renderLayer), shouldRender(true), engine2D(engine), VisibleComponentBase(engine)
{
	if (engine2D == 0)
		engine2D = r2Engine;
}

Visible2DComponent::~Visible2DComponent()
{
	if (program)
	{
		engine2D->RemoveComponent(this);
	}
}

void Visible2DComponent::SetShaderProgram(ShaderProgramBase * program)
{
	if (this->program)
	{
		engine2D->RemoveComponent(this);
	}

	this->program = program;
	engine2D->AddComponent(this);
}

void Visible2DComponent::SetRenderLayer(unsigned layer)
{
	renderLayer = layer;
	if (program)
	{
		//TODO: maybe find more optimized way to re sort mwhen moving between layers
		engine2D->RemoveComponent(this);
		engine2D->AddComponent(this);
	}
}

void Visible2DComponent::PreRender()
{
	if (isDirty)
	{
		CalculateLocalMatrix();
		isDirty = false;
	}
}

void Visible2DComponent::Render(const Matrix44& cameraMatrix)
{
	program->SetMatricies(cameraMatrix, GetWorldMatrix());

	if(shouldRender)
		renderedObject->RenderObject(cameraMatrix);
}

void Visible2DComponent::PostRender()
{
}
