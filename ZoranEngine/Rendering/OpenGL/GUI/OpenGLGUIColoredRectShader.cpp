#include "stdafx.h"
#include "OpenGLGUIColoredRectShader.h"

#include <GL/glew.h>

const ShaderInitMap* OpenGLGUIColoredRectShader::initMap = (new ShaderInitMap(
	{
		{ GL_VERTEX_SHADER, "Shaders/GUI/guiColoredRect.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/GUI/guiColoredRect.frag" }
	}));


OpenGLGUIColoredRectShader::OpenGLGUIColoredRectShader(const ShaderInitMap* initMap) : OpenGLShaderProgramBase(initMap)
{
}

OpenGLGUIColoredRectShader::~OpenGLGUIColoredRectShader()
{
}
