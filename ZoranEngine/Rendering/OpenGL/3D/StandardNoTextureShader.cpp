#include "stdafx.h"
#include "StandardNoTextureShader.h"

#include <GL/glew.h>

ShaderInitMap* StandardNoTextureShader::initMap = new ShaderInitMap(
{
	{ GL_VERTEX_SHADER, "Shaders/3D/standardNoTexture.vert" },
	{ GL_FRAGMENT_SHADER, "Shaders/3D/standardNoTexture.frag" }
});

StandardNoTextureShader::StandardNoTextureShader(const ShaderInitMap* initMap) : OpenGLShaderProgramBase(initMap)
{
}


StandardNoTextureShader::~StandardNoTextureShader()
{
}
