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
class Vector2I;
class Vector2D;
class Vector3D;
class ZoranEngine_EXPORT ShaderProgramBase
{
protected:
	RenderEngineBase * engine;
public:
	ShaderProgramBase();
	virtual ~ShaderProgramBase();

	virtual void SetMatricies(const Matrix44& cameraMatrix, const Matrix44& world) = 0;

	virtual void BindProgram() = 0;
	virtual void UnBindProgram() = 0;

	virtual void setUniform(const char* uniform, unsigned int value) = 0;
	virtual void setUniform(const char* uniform, int value) = 0;
	virtual void setUniform(const char* uniform, float value) = 0;
	virtual void setUniform(const char* uniform, double value) = 0;
	virtual void setUniform(const char* uniform, Vector2I* value) = 0;
	virtual void setUniform(const char* uniform, Vector2D* value) = 0;
	virtual void setUniform(const char* uniform, Vector3D* value) = 0;
	virtual void setUniform(const char* uniform, float value1, float value2, float value3, float value4) = 0;
	virtual void setUniformMat4(const char* uniform, const float *value) = 0;
	virtual void setUniformMat2(const char * uniform, const float *value) = 0;
	virtual void setUniformMat4(const char* uniform, const double *value) = 0;
	virtual void setUniformMat2(const char * uniform, const double *value) = 0;
};

