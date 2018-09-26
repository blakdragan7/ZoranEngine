#pragma once
#include <Rendering\OpenGL\OpenGLShaderProgramBase.h>

class ZoranEngine_EXPORT StandardShader3D : public OpenGLShaderProgramBase
{
public:
	StandardShader3D(const ShaderInitMap* initMap);
	~StandardShader3D();

	virtual void SetModelMatrix(const Matrix44& world)override;

	static const ShaderInitMap* initMap;
};

