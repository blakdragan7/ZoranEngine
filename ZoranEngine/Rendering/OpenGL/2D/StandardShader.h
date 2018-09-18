#pragma once
#include <Rendering/OpenGL/OpenGLShaderProgramBase.h>
class ZoranEngine_EXPORT StandardShader : public OpenGLShaderProgramBase
{
public:
	StandardShader(const ShaderInitMap*);
	~StandardShader();

	virtual void SetModelMatrix(const Matrix44& world)override;

	static const ShaderInitMap* initMap;
};
