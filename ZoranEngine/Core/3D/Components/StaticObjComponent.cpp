#include "stdafx.h"
#include "StaticObjComponent.h"

#include <Rendering/RenderEngineBase.h>
#include <Rendering/Renderers/ModelRenderer.h>
#include <Rendering/Renderers/TriangleStripRenderer.h>

#include <Rendering/OpenGL/3D/StandardShader3D.h>

StaticObjComponent::StaticObjComponent()
{
	modelRenderer = rEngine->CreateModelRenderer();

	test = rEngine->CreateTriangleStripRenderer();

	test->MakeFullScreenQuad();

	ShaderProgramBase* program = rEngine->CreateShaderProgram<StandardShader3D>(StandardShader3D::initMap);

	SetShaderProgram(program);
}

StaticObjComponent::StaticObjComponent(const char * file)
{
	modelRenderer = rEngine->CreateModelRenderer();

	test = rEngine->CreateTriangleStripRenderer();

	test->MakeFullScreenQuad();

	ShaderProgramBase* program = rEngine->CreateShaderProgram<StandardShader3D>(StandardShader3D::initMap);

	SetShaderProgram(program);

	LoadFile(file);
}

StaticObjComponent::~StaticObjComponent()
{
	delete modelRenderer;
}

void StaticObjComponent::LoadFile(const char * file)
{
	modelRenderer->LoadFile(file);
}

void StaticObjComponent::PreRender()
{
	Visible3DComponent::PreRender();

	GetShaderProgram()->SetModelMatrix(GetWorldMatrix());
}

void StaticObjComponent::Render()
{
	//test->RenderObject();
	modelRenderer->RenderObject();
}
