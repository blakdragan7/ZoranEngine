#pragma once
#include <Rendering\OpenGL\OpenGLShaderProgramBase.h>

class OpenGLFontShader : public OpenGLShaderProgramBase
{
public:
	OpenGLFontShader(const ShaderInitMap* initMap);
	~OpenGLFontShader();

	static const ShaderInitMap* initMap;
};

