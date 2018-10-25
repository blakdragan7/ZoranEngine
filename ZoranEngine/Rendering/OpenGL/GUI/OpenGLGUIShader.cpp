#include "stdafx.h"
#include "OpenGLGUIShader.h"

#include <GL/glew.h>

const ShaderInitMap* OpenGLGUIShader::initMap = (new ShaderInitMap(
	{
		{ GL_VERTEX_SHADER, "Shaders/GUI/gui.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/GUI/gui.frag" }
	}));

OpenGLGUIShader::OpenGLGUIShader(const ShaderInitMap* initMap) : OpenGLShaderProgramBase(initMap)
{
}


OpenGLGUIShader::~OpenGLGUIShader()
{
}
