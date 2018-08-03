#pragma once
#include "OpenGLShaderProgramBase.h"
class ZoranEngine_EXPORT StandardShader : public OpenGLShaderProgramBase
{
public:
	StandardShader();
	~StandardShader();

	virtual bool SetupShaderFromSceneObject(SceneObject* object)override;
};
