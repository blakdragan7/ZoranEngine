#pragma once
#include "Core/PlatformTypes.h"

/*
* ShaderProgramBase must be sub classed To implement specific shaders.
* There is not really shared logic in the baseclass, it's only a base class to 
* allow easy abstract passing between the scene object and the render engine
*/
class SceneObject;
class RenderEngineBase;
class ZoranEngine_EXPORT ShaderProgramBase
{
protected:
	RenderEngineBase * engine;
public:
	ShaderProgramBase();
	virtual ~ShaderProgramBase();

	virtual bool SetupShaderFromSceneObject(SceneObject* object) = 0;
	virtual void BindProgram() = 0;
	virtual void UnBindProgram() = 0;
};

