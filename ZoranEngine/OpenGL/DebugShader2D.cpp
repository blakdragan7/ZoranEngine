#include "stdafx.h"
#include "DebugShader2D.h"
#include "Core/CameraBase.h"
#include "GL/glew.h"
#include "Core/SceneObject.h"

DebugShader2D::DebugShader2D()
{
	AddShaderFromSource("Shaders/2D/standard.vert", GL_VERTEX_SHADER);
	AddShaderFromSource("Shaders/2D/standard_debug.frag", GL_FRAGMENT_SHADER);
	Link();
}

DebugShader2D::~DebugShader2D()
{
}

bool DebugShader2D::SetupShaderFromSceneObject(SceneObject * object)
{
	MatrixF model = object->GetModel();
	model = zEngine->GetCamera()->GetModel() * model;
	setUniformMat4("MVP",&model[0]);
	return true;
}