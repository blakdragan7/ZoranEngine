#pragma once
#include "ShaderProgramBase.h"

class Vector2D;
class Vector2L;
class Vector3D;
class SceneObject;
class DRAGENGINE_EXPORT OpenGLShaderProgramBase : public ShaderProgramBase
{
private:
	unsigned program;

public:
	OpenGLShaderProgramBase();
	virtual ~OpenGLShaderProgramBase();

	bool Link();

	bool AddShaderFromSource(const char* source, unsigned type);
	bool AddShaderFromConst(const char* const_, unsigned type);

	void setUniform(const char* uniform, unsigned int value);
	void setUniform(const char* uniform, int value);
	void setUniform(const char* uniform, float value);
	void setUniform(const char* uniform, double value);
	void setUniform(const char* uniform, Vector2L* value);
	void setUniform(const char* uniform, Vector2D* value);
	void setUniform(const char* uniform, Vector3D* value);
	void setUniform(const char* uniform, float value1, float value2, float value3, float value4);
	void setUniformMat4(const char* uniform, float *value);
	void setUniformMat2(const char * uniform, float *value);
	void setUniformMat4(const char* uniform, double *value);
	void setUniformMat2(const char * uniform, double *value);

	void setPatchVertexCount(unsigned int count);

	unsigned getAttrLocation(const char* attr);

	virtual void BindProgram()override;
	virtual void UnBindProgram()override;
};

