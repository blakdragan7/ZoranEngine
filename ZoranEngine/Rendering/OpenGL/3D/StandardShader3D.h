#pragma once
#include <Rendering\OpenGL\OpenGLShaderProgramBase.h>

class ZoranEngine_EXPORT StandardShader3D : public OpenGLShaderProgramBase
{
public:
	StandardShader3D(const ShaderInitMap* initMap);
	~StandardShader3D();

	static const ShaderInitMap* initMap;
};

