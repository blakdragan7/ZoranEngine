#pragma once
#include "Core/PlatformTypes.h"
#include <unordered_map>
/*
* ShaderProgramBase must be sub classed To implement specific shaders.
* There is not really shared logic in the baseclass, it's only a base class to 
* allow easy abstract passinfg between the scene object and the render engine
*/
typedef std::unordered_map<unsigned, const char*> ShaderInitMap;

class RenderEngineBase;
class Matrix44;
class ZoranEngine_EXPORT ShaderProgramBase
{
protected:
	RenderEngineBase * engine;
public:
	ShaderProgramBase();
	virtual ~ShaderProgramBase();

	virtual void SetModelMatrix(const Matrix44& world) = 0;

	virtual void BindProgram() = 0;
	virtual void UnBindProgram() = 0;
};

