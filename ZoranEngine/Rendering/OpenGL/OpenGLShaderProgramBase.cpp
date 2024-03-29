#include "stdafx.h"
#include "OpenGLShaderProgramBase.h"
#include <GL/glew.h>
#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Math/Matrix44.h>
#include <Rendering/RenderEngineBase.h>
#include <Rendering/Primitives.h>

#include <fstream>

#include <assert.h>

OpenGLShaderProgramBase* OpenGLShaderProgramBase::CurrentlyBoundShader = 0;

void OpenGLShaderProgramBase::DeleteAllShaders()
{
	for (unsigned i = 0; i < currentShaderIndex; i++)
	{
		glDeleteShader(shaders[i]);
		shaders[i] = 0;
	}
}

OpenGLShaderProgramBase::OpenGLShaderProgramBase() : currentShaderIndex(0)
{
	memset(shaders,0,sizeof(shaders));

	program = glCreateProgram();
	engine->CheckErrors("OpenGLShaderProgramBase()");
}

OpenGLShaderProgramBase::OpenGLShaderProgramBase(const ShaderInitMap* initMap)
{
	program = glCreateProgram();
	engine->CheckErrors("OpenGLShaderProgramBase()");

	for (auto iter : *initMap)
	{
		if (AddShaderFromSource(iter.second, iter.first) == false)
		{
			Log(LogLevel_Error, "Failed to add shader %s", iter.second);
		}
	}

	if (Link() == false)
	{
		Log(LogLevel_Error,"Failed To Link OepnGL Shader Program !!");
	}
}


OpenGLShaderProgramBase::~OpenGLShaderProgramBase()
{
	glDeleteProgram(program);
	//engine->CheckErrors("~OpenGLShaderProgramBase()");
}

void OpenGLShaderProgramBase::BindProgram()
{
	if (CurrentlyBoundShader == this)return;

	glUseProgram(program);
	engine->CheckErrors("BindProgram()");
	CurrentlyBoundShader = this;
}

void OpenGLShaderProgramBase::UnBindProgram()
{
	if (CurrentlyBoundShader != this)
	{
		Log(LogLevel_Error, "Trying To Unbind Shader that isnt Bound !!\n");
		return;
	}
	glUseProgram(0);
	CurrentlyBoundShader = 0;
}

void OpenGLShaderProgramBase::SetMatricies(const Matrix44 & cameraMatrix, const Matrix44 & world)
{
	Matrix44 model(false);
	model = cameraMatrix * world;
	setUniformMat4("MVP", &model[0]);
}

bool OpenGLShaderProgramBase::Link()
{
	GLint status = GL_FALSE;
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		GLchar* infoLog = new GLchar[maxLength];
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
		Log(LogLevel_Error,"Program Link Error: %s\n", infoLog);

		DeleteAllShaders();

		return false;
	}

	DeleteAllShaders();
	return true;
}

bool OpenGLShaderProgramBase::AddShaderFromSource(const char * source, unsigned type)
{
	assert(currentShaderIndex < 11 && "Can not add another shader !!");

	std::fstream File;
	File.open(source, std::ios::in);
	bool good = File.good();
	if (!good)
	{
		Log(LogLevel_Error, "Error File not found:  %s\n", source);
		return false;
	}
	std::string cs;
	File.seekg(0, std::ios::end);
	cs.resize((unsigned int)File.tellg());
	File.seekg(0, std::ios::beg);
	File.read(&cs[0], cs.size());
	File.close();
	const char* temp = cs.c_str();
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &temp, NULL);
	glCompileShader(shader);

	GLint isCompiled = 1;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		GLchar* infoLog = new GLchar[maxLength];
		glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
		Log(LogLevel_Error, "%s Compile Error: %s\n",source,infoLog);
		glDeleteShader(shader);
		return false;
	}
	glAttachShader(program, shader);

	shaders[currentShaderIndex] = shader;
	++currentShaderIndex;

	return true;
}

bool OpenGLShaderProgramBase::AddShaderFromConst(const char* const_, unsigned type)
{
	assert(currentShaderIndex < 11 && "Can not add another shader !!");

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &const_, NULL);
	glCompileShader(shader);

	GLint isCompiled = 1;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		char* infoLog = new char[maxLength];
		glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
		Log(LogLevel_Error, "Shader Compile Error: %s\n", infoLog);

		glDeleteShader(shader);
		return false;
	}
	glAttachShader(program, shader);

	shaders[currentShaderIndex] = shader;
	++currentShaderIndex;

	return true;
}

void OpenGLShaderProgramBase::setUniform(const char* uniform, unsigned int value)
{
	GLuint loc = glGetUniformLocation(program, uniform);
	glUniform1ui(loc, value);
	engine->CheckErrors("setUniform()");
}

void OpenGLShaderProgramBase::setUniform(const char* uniform, int value)
{
	GLuint loc = glGetUniformLocation(program, uniform);
	glUniform1i(loc, value);
	engine->CheckErrors("setUniform()");
}

void OpenGLShaderProgramBase::setUniform(const char* uniform, float value)
{
	GLuint loc = glGetUniformLocation(program, uniform);
	glUniform1f(loc, value);
	engine->CheckErrors("setUniform()");
}

void OpenGLShaderProgramBase::setUniform(const char* uniform, double value)
{
	GLuint loc = glGetUniformLocation(program, uniform);
	glUniform1d(loc, value);
	engine->CheckErrors("setUniform()");
}


void OpenGLShaderProgramBase::setUniform(const char* uniform, Vector2I* value)
{
	GLuint loc = glGetUniformLocation(program, uniform);
	glUniform2i(loc, (int)value->x, (int)value->y);
	engine->CheckErrors("setUniform()");
}

void OpenGLShaderProgramBase::setUniform(const char* uniform, Vector2D* value)
{
	GLuint loc = glGetUniformLocation(program, uniform);
	glUniform2f(loc, value->x, value->y);
	engine->CheckErrors("setUniform()");
}

void OpenGLShaderProgramBase::setUniform(const char* uniform, Vector3D* value)
{
	engine->CheckErrors("");
	GLuint loc = glGetUniformLocation(program, uniform);
	glUniform3f(loc, value->x, value->y, value->z);
	engine->CheckErrors("setUniform()");
}

void OpenGLShaderProgramBase::setUniform(const char * uniform, Color * value)
{
	engine->CheckErrors("");
	GLuint loc = glGetUniformLocation(program, uniform);
	glUniform4f(loc, value->r, value->g, value->b, value->a);
	engine->CheckErrors("setUniform()");
}

void OpenGLShaderProgramBase::setUniform(const char* uniform, float value1, float value2, float value3, float value4)
{
	GLuint loc = glGetUniformLocation(program, uniform);
	glUniform4f(loc, value1, value2, value3, value4);
	engine->CheckErrors("setUniform()");
}
void OpenGLShaderProgramBase::setUniformMat4(const char * uniform, const float * value)
{
	GLuint loc = glGetUniformLocation(program, uniform);
	glUniformMatrix4fv(loc, 1, true, value);
	engine->CheckErrors("setUniformMat4()");
}

void OpenGLShaderProgramBase::setUniformMat2(const char * uniform, const float * value)
{
	GLuint loc = glGetUniformLocation(program, uniform);
	glUniformMatrix2fv(loc, 1, true, value);
	engine->CheckErrors("setUniformMat2()");
}

void OpenGLShaderProgramBase::setUniformMat4(const char * uniform, const double * value)
{
	GLuint loc = glGetUniformLocation(program, uniform);
	glUniformMatrix4dv(loc, 1, true, value);
	engine->CheckErrors("setUniformMat4()");
}

void OpenGLShaderProgramBase::setUniformMat2(const char * uniform, const double * value)
{
	GLuint loc = glGetUniformLocation(program, uniform);
	glUniformMatrix2dv(loc, 1, true, value);
	engine->CheckErrors("setUniformMat2()");
}

unsigned OpenGLShaderProgramBase::getAttrLocation(const char * attr)
{
	unsigned att = glGetAttribLocation(program, attr);
	engine->CheckErrors("getAttrLocation()");
	return att;
}

void OpenGLShaderProgramBase::setPatchVertexCount(unsigned int count)
{
	glPatchParameteri(GL_PATCH_VERTICES, count);
	engine->CheckErrors("setPatchVertexCount()");
}
