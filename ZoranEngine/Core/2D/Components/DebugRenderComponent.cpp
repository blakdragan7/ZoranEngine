#include "stdafx.h"
#include "DebugRenderComponent.h"
#include <Rendering/RenderEngineBase.h>
#include <OpenGL\DebugShader2D.h>
#include <Rendering/RenderedObjectBase.h>

DebugRenderComponent::DebugRenderComponent()
{
	shader = static_cast<DebugShader2D*>(rEngine->CreateShaderProgram<DebugShader2D>(DebugShader2D::initMap));

	renderedObject->MakeFullScreenQuad();

	SetShaderProgram(shader);
}


DebugRenderComponent::~DebugRenderComponent()
{
}

void DebugRenderComponent::PreRender()
{
	shader->setUniform("Color",&color);

	GetShaderProgram()->SetModelMatrix(GetWorldMatrix());
}
