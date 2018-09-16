#include "stdafx.h"
#include "StandardShader.h"
#include "Core/CameraBase.h"
#include "GL/glew.h"
#include "Core/SceneObject.h"

const ShaderInitMap* StandardShader::initMap = (new ShaderInitMap(
{
	{ GL_VERTEX_SHADER, "Shaders/standard.vert" },
	{ GL_FRAGMENT_SHADER, "Shaders/standard.frag" }
}));

StandardShader::StandardShader(const ShaderInitMap* initMap) : OpenGLShaderProgramBase(initMap)
{
}


StandardShader::~StandardShader()
{
}

void StandardShader::SetModelMatrix(const Matrix44 & world)
{
	Matrix44 model(false);
	model = zEngine->GetCamera()->GetModel() * world;
	setUniformMat4("MVP", &model[0]);
}

