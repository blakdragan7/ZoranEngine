#pragma once
#include <Rendering\OpenGL\OpenGLShaderProgramBase.h>

class OpenGLSDFFontShader : public OpenGLShaderProgramBase
{
public:
	OpenGLSDFFontShader(const ShaderInitMap* initMap);
	~OpenGLSDFFontShader();

	static const ShaderInitMap* initMap;
};

class OpenGLMSDFFontShader : public OpenGLShaderProgramBase
{
public:
	OpenGLMSDFFontShader(const ShaderInitMap* initMap);
	~OpenGLMSDFFontShader();

	static const ShaderInitMap* initMap;
};

