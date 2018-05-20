#pragma once
#include "OpenGLShaderProgramBase.h"
class StandardShader : public OpenGLShaderProgramBase
{
public:
	StandardShader();
	~StandardShader();

	virtual bool SetupShaderFromSceneObject(SceneObject* object)override;
};

