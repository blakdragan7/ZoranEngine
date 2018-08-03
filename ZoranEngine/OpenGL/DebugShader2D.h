#pragma once
#include "OpenGLShaderProgramBase.h"

class ZoranEngine_EXPORT DebugShader2D : public OpenGLShaderProgramBase
{
	class DebugSceneObject2D* debugObject;
public:
	DebugShader2D(DebugSceneObject2D* object);
	~DebugShader2D();

	virtual bool SetupShaderFromSceneObject(SceneObject* object)override;
};
