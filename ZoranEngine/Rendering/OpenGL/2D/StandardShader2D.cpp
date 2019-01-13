#include "stdafx.h"
#include "StandardShader2D.h"
#include "Core/CameraBase.h"
#include "GL/glew.h"
#include "Core/SceneObject.h"

#include <Utils/Statistics.h>

const ShaderInitMap* StandardShader2D::initMap = (new ShaderInitMap(
{
	{ GL_VERTEX_SHADER, "Shaders/2D/standard.vert" },
	{ GL_FRAGMENT_SHADER, "Shaders/2D/standard.frag" }
}));

StandardShader2D::StandardShader2D(const ShaderInitMap* initMap) : OpenGLShaderProgramBase(initMap)
{
}

StandardShader2D::~StandardShader2D()
{
}
