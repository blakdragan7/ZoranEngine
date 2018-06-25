#pragma once
#include "OpenGLShaderProgramBase.h"

class ZoranEngine_EXPORT StandardShader2D : public OpenGLShaderProgramBase
{
public:
	StandardShader2D();
	~StandardShader2D();

	virtual bool SetupShaderFromSceneObject(SceneObject* object)override;
};
