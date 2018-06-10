#include "stdafx.h"
#include "StandardShader.h"
#include "CameraBase.h"
#include "GL/glew.h"
#include "SceneObject.h"

StandardShader::StandardShader()
{
	AddShaderFromSource("Shaders/standard.vert", GL_VERTEX_SHADER);
	AddShaderFromSource("Shaders/standard.frag", GL_FRAGMENT_SHADER);
	Link();
}


StandardShader::~StandardShader()
{
}

bool StandardShader::SetupShaderFromSceneObject(SceneObject * object)
{

	MatrixF model = object->GetModel();
	model = dEngine->GetCamera()->GetModel() * model;
	setUniformMat4("MVP",&model[0]);
	return true;
}
