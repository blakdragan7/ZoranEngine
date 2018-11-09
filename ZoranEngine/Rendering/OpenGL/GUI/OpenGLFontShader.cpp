#include "stdafx.h"
#include "OpenGLFontShader.h"

#include <GL/glew.h>

const ShaderInitMap* OpenGLSDFFontShader::initMap = (new ShaderInitMap(
	{
		{ GL_VERTEX_SHADER, "Shaders/GUI/font.vert" },
	{ GL_FRAGMENT_SHADER, "Shaders/GUI/fontSDF.frag" }
	}));

OpenGLSDFFontShader::OpenGLSDFFontShader(const ShaderInitMap* initMap) : OpenGLShaderProgramBase(initMap)
{
}

OpenGLSDFFontShader::~OpenGLSDFFontShader()
{
}

const ShaderInitMap* OpenGLMSDFFontShader::initMap = (new ShaderInitMap(
	{
		{ GL_VERTEX_SHADER, "Shaders/GUI/font.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/GUI/fontMSDF.frag" }
	}));

OpenGLMSDFFontShader::OpenGLMSDFFontShader(const ShaderInitMap* initMap) : OpenGLShaderProgramBase(initMap)
{
}

OpenGLMSDFFontShader::~OpenGLMSDFFontShader()
{
}
