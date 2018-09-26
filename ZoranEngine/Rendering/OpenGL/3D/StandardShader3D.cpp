#include "stdafx.h"
#include "StandardShader3D.h"

#include <GL/glew.h>

#include <Math/Matrix44.hpp>
#include <Core/ZoranEngine.h>
#include <Core/CameraBase.h>

const ShaderInitMap* StandardShader3D::initMap = (new ShaderInitMap(
	{
		{ GL_VERTEX_SHADER, "Shaders/3D/standard.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/3D/standard.frag" }
	}));

StandardShader3D::StandardShader3D(const ShaderInitMap* initMap) : OpenGLShaderProgramBase(initMap)
{
}

StandardShader3D::~StandardShader3D()
{
}

void StandardShader3D::SetModelMatrix(const Matrix44 & world)
{
	Matrix44 model(false);
	model = zEngine->GetCamera()->GetModel() * world;
	setUniformMat4("MVP", &model[0]);
}
