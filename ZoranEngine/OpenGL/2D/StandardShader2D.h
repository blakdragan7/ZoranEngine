#pragma once
#include <OpenGL/OpenGLShaderProgramBase.h>

class ZoranEngine_EXPORT StandardShader2D : public OpenGLShaderProgramBase
{
public:
	StandardShader2D(const ShaderInitMap* initMap);
	~StandardShader2D();

	virtual void SetModelMatrix(const Matrix44& world)override;

	static const ShaderInitMap* initMap;
};
