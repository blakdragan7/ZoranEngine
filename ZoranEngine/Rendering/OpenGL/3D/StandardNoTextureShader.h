#pragma once
#include <Rendering\OpenGL\OpenGLShaderProgramBase.h>

class StandardNoTextureShader : public OpenGLShaderProgramBase
{
public:
	StandardNoTextureShader(const ShaderInitMap* initMap);
	~StandardNoTextureShader();

	static ShaderInitMap* initMap;
};

