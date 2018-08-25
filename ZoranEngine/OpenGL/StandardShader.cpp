#include "stdafx.h"
#include "StandardShader.h"
#include "Core/CameraBase.h"
#include "GL/glew.h"
#include "Core/SceneObject.h"

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

	Matrix44 model = object->GetModel();
	model = zEngine->GetCamera()->GetModel() * model;
	setUniformMat4("MVP",&model[0]);
	
	return true;
}
