#pragma once
#include "OpenGLShaderProgramBase.h"

class ZoranEngine_EXPORT DebugShader2D : public OpenGLShaderProgramBase
{
public:
	DebugShader2D(const ShaderInitMap* initMap);
	~DebugShader2D();

	virtual void SetModelMatrix(const Matrix44& world)override;

	static const ShaderInitMap* initMap;
};
