#pragma once
#include <Rendering/OpenGL/OpenGLShaderProgramBase.h>

class ZoranEngine_EXPORT DebugShader2D : public OpenGLShaderProgramBase
{
public:
	DebugShader2D(const ShaderInitMap* initMap);
	~DebugShader2D();

	static const ShaderInitMap* initMap;
};
