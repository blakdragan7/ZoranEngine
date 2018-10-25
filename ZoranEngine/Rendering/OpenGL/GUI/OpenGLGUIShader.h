#pragma once
#include <Rendering\OpenGL\OpenGLShaderProgramBase.h>

class OpenGLGUIShader : public OpenGLShaderProgramBase
{
public:
	OpenGLGUIShader(const ShaderInitMap* initMap);
	~OpenGLGUIShader();

	static const ShaderInitMap* initMap;
};

