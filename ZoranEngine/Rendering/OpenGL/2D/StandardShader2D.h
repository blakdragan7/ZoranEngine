#pragma once
#include <Rendering/OpenGL/OpenGLShaderProgramBase.h>

class ZoranEngine_EXPORT StandardShader2D : public OpenGLShaderProgramBase
{
public:
	StandardShader2D(const ShaderInitMap* initMap);
	~StandardShader2D();

	static const ShaderInitMap* initMap;
};
