#include "stdafx.h"
#include "StandardShader3D.h"

#include <GL/glew.h>

#include <Math/Matrix44.h>
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
