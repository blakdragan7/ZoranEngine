#pragma once
#include <Rendering\OpenGL\OpenGLShaderProgramBase.h>

class OpenGLGUIColoredRectShader : public OpenGLShaderProgramBase
{
public:
	OpenGLGUIColoredRectShader(const ShaderInitMap* initMap);
	~OpenGLGUIColoredRectShader();

	static const ShaderInitMap* initMap;
};

