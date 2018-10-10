#pragma once
#include "Rendering/ShaderProgramBase.h"
#include <map>

class Vector2D;
class Vector2I;
class Vector3D;
class SceneObject;
class ZoranEngine_EXPORT OpenGLShaderProgramBase : public ShaderProgramBase
{
private:
	unsigned shaders[12];
	unsigned program;

	unsigned currentShaderIndex;

	static OpenGLShaderProgramBase* CurrentlyBoundShader;

private:
	// used to free up mempry after program is linked
	void DeleteAllShaders();

public:
	OpenGLShaderProgramBase();
	OpenGLShaderProgramBase(const ShaderInitMap* initMap);
	virtual ~OpenGLShaderProgramBase();

	bool Link();

	bool AddShaderFromSource(const char* source, unsigned type);
	bool AddShaderFromConst(const char* const_, unsigned type);

	virtual void setUniform(const char* uniform, unsigned int value)override;
	virtual void setUniform(const char* uniform, int value)override;
	virtual void setUniform(const char* uniform, float value)override;
	virtual void setUniform(const char* uniform, double value)override;
	virtual void setUniform(const char* uniform, Vector2I* value)override;
	virtual void setUniform(const char* uniform, Vector2D* value)override;
	virtual void setUniform(const char* uniform, Vector3D* value)override;
	virtual void setUniform(const char* uniform, float value1, float value2, float value3, float value4)override;
	virtual void setUniformMat4(const char* uniform, const float *value)override;
	virtual void setUniformMat2(const char * uniform, const float *value)override;
	virtual void setUniformMat4(const char* uniform, const double *value)override;
	virtual void setUniformMat2(const char * uniform, const double *value)override;

	void setPatchVertexCount(unsigned int count);

	unsigned getAttrLocation(const char* attr);

	virtual void BindProgram()override;
	virtual void UnBindProgram()override;

	void SetMatricies(const Matrix44& cameraMatrix, const Matrix44& world)override;
};

