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

	void setUniform(const char* uniform, unsigned int value);
	void setUniform(const char* uniform, int value);
	void setUniform(const char* uniform, float value);
	void setUniform(const char* uniform, double value);
	void setUniform(const char* uniform, Vector2I* value);
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

