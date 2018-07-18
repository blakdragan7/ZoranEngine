#pragma once
#include "OpenGLShaderProgramBase.h"

class ZoranEngine_EXPORT DebugShader2D : public OpenGLShaderProgramBase
{
public:
	DebugShader2D();
	~DebugShader2D();

	virtual bool SetupShaderFromSceneObject(SceneObject* object)override;
};
