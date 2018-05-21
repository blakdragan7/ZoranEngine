#pragma once
#include "OpenGLShaderProgramBase.h"
class DRAGENGINE_EXPORT StandardShader : public OpenGLShaderProgramBase
{
public:
	StandardShader();
	~StandardShader();

	virtual bool SetupShaderFromSceneObject(SceneObject* object)override;
};

