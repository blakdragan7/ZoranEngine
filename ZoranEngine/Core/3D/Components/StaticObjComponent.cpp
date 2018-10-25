#include "stdafx.h"
#include "StaticObjComponent.h"

#include <Rendering/RenderEngineBase.h>
#include <Rendering/Renderers/ModelRenderer.h>
#include <Rendering/Renderers/TriangleStripRenderer.h>

#include <Rendering/OpenGL/3D/StandardShader3D.h>
#include <Rendering/OpenGL/3D/StandardNoTextureShader.h>

StaticObjComponent::StaticObjComponent()
{
	modelRenderer = rEngine->CreateModelRenderer();

	test = rEngine->CreateTriangleStripRenderer();

	test->MakeFullScreenQuad();

	ShaderProgramBase* program = rEngine->CreateShaderProgram<StandardNoTextureShader>();

	SetShaderProgram(program);
}

StaticObjComponent::StaticObjComponent(const char * file)
{
	modelRenderer = rEngine->CreateModelRenderer();

	test = rEngine->CreateTriangleStripRenderer();

	test->MakeFullScreenQuad();

	ShaderProgramBase* program = rEngine->CreateShaderProgram<StandardShader3D>();

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

	program->setUniform("matColor", &Vector3D(0,0,1));
}

void StaticObjComponent::Render(const Matrix44& cameraMatrix)
{
	program->SetMatricies(cameraMatrix, GetWorldMatrix());
	//test->RenderObject();
	modelRenderer->RenderObject(cameraMatrix);
}
