#include "stdafx.h"
#include "OpenGLFontShader.h"

#include <GL/glew.h>

const ShaderInitMap* OpenGLFontShader::initMap = (new ShaderInitMap(
	{
		{ GL_VERTEX_SHADER, "Shaders/GUI/font.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/GUI/font.frag" }
	}));

OpenGLFontShader::OpenGLFontShader(const ShaderInitMap* initMap) : OpenGLShaderProgramBase(initMap)
{
}

OpenGLFontShader::~OpenGLFontShader()
{
}
