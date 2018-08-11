#include "stdafx.h"
#include "StandardShader2D.h"
#include "Core/CameraBase.h"
#include "GL/glew.h"
#include "Core/SceneObject.h"

#include <Utils/Statistics.h>

StandardShader2D::StandardShader2D()
{
	AddShaderFromSource("Shaders/2D/standard.vert", GL_VERTEX_SHADER);
	AddShaderFromSource("Shaders/2D/standard.frag", GL_FRAGMENT_SHADER);
	Link();
}

StandardShader2D::~StandardShader2D()
{
}

bool StandardShader2D::SetupShaderFromSceneObject(SceneObject * object)
{
	MatrixF model = object->GetModel();
	model = zEngine->GetCamera()->GetModel() * model;
	setUniformMat4("MVP",&model[0]);
	return true;
}