#pragma once
#include "OpenGLShaderProgramBase.h"

class ZoranEngine_EXPORT StandardShader2D : public OpenGLShaderProgramBase
{
public:
	StandardShader2D(ShaderInitMap initMap);
	~StandardShader2D();

	virtual void SetModelMatrix(const Matrix44& world)override;

	static const ShaderInitMap initMap;
};
