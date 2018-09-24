#include "stdafx.h"
#include "DebugRenderComponent.h"
#include <Rendering/RenderEngineBase.h>
#include <Rendering/OpenGL/2D/DebugShader2D.h>
#include <Rendering/RenderedObjectBase.h>
#include <Rendering/Renderers/SpriteRenderer.h>

DebugRenderComponent::DebugRenderComponent() : Visible2DComponent(200)
{
	shader = static_cast<DebugShader2D*>(rEngine->CreateShaderProgram<DebugShader2D>(DebugShader2D::initMap));

	renderedObject = rEngine->CreateSpriteRenderer();

	SetShaderProgram(shader);
	SetScale({ 2,2 });
}


DebugRenderComponent::~DebugRenderComponent()
{
}

void DebugRenderComponent::PreRender()
{
	shader->setUniform("Color",&color);
	CalculateLocalMatrix();
	GetShaderProgram()->SetModelMatrix(GetWorldMatrix());
}
