#include "stdafx.h"
#include "StaticObjComponent.h"

#include <Rendering/RenderEngineBase.h>
#include <Rendering/Renderers/ModelRenderer.h>
#include <Rendering/Renderers/TriangleStripRenderer.h>

#include <Rendering/OpenGL/3D/StandardShader3D.h>
#include <Rendering/OpenGL/3D/StandardNoTextureShader.h>

#include <Resources/ResourceManager.h>

StaticObjComponent::StaticObjComponent()
{
	test = rEngine->CreateTriangleStripRenderer();

	test->MakeFullScreenQuad();

	ShaderProgramBase* program = rEngine->CreateShaderProgram<StandardNoTextureShader>();

	SetShaderProgram(program);
}

StaticObjComponent::StaticObjComponent(const char * file)
{
	test = rEngine->CreateTriangleStripRenderer();

	test->MakeFullScreenQuad();

	ShaderProgramBase* program = rEngine->CreateShaderProgram<StandardShader3D>();

	SetShaderProgram(program);

	LoadFile(file);
}

StaticObjComponent::StaticObjComponent(ModelResource model)
{
	test->MakeFullScreenQuad();

	ShaderProgramBase* program = rEngine->CreateShaderProgram<StandardShader3D>();

	SetShaderProgram(program);

	this->model = model;
}

StaticObjComponent::~StaticObjComponent()
{
}

void StaticObjComponent::LoadFile(const char * file)
{
	model = RM->ModelForPath(file);
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
	model->RenderObject(cameraMatrix);
}
