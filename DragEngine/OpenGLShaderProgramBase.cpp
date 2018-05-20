#include "stdafx.h"
#include "GL/glew.h"
#include "OpenGLShaderProgramBase.h"
#include "Vector2.h"
#include "Vector3.h"

#include <iostream>
OpenGLShaderProgramBase::OpenGLShaderProgramBase()
{
	program = glCreateProgram();
}


OpenGLShaderProgramBase::~OpenGLShaderProgramBase()
{
	glDeleteProgram(program);
}

void OpenGLShaderProgramBase::BindProgram()
{
	glUseProgram(program);
}

void OpenGLShaderProgramBase::UnBindProgram()
{
	glUseProgram(0);
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
		std::cout << "Program Link Error: " << infoLog << std::endl;

		return false;
	}
	return true;
}

void OpenGLShaderProgramBase::setUniform(const char* uniform, unsigned int value)
{
	GLuint loc = glGetUniformLocation(program, uniform);
	glUniform1ui(loc, value);
}

void OpenGLShaderProgramBase::setUniform(const char* uniform, int value)
{
	GLuint loc = glGetUniformLocation(program, uniform);
	glUniform1i(loc, value);
}

void OpenGLShaderProgramBase::setUniform(const char* uniform, float value)
{
	GLuint loc = glGetUniformLocation(program, uniform);
	glUniform1f(loc, value);
}

void OpenGLShaderProgramBase::setUniform(const char* uniform, double value)
{
	GLuint loc = glGetUniformLocation(program, uniform);
	glUniform1d(loc, value);
}


void OpenGLShaderProgramBase::setUniform(const char* uniform, Vector2L* value)
{
	GLuint loc = glGetUniformLocation(program, uniform);
	glUniform2i(loc, value->x, value->y);
}

void OpenGLShaderProgramBase::setUniform(const char* uniform, Vector2D* value)
{
	GLuint loc = glGetUniformLocation(program, uniform);
	glUniform2d(loc, value->x, value->y);
}

void OpenGLShaderProgramBase::setUniform(const char* uniform, Vector3D* value)
{
	GLuint loc = glGetUniformLocation(program, uniform);
	glUniform3d(loc, value->x, value->y, value->z);
}

void OpenGLShaderProgramBase::setUniform(const char* uniform, float value1, float value2, float value3, float value4)
{
	GLuint loc = glGetUniformLocation(program, uniform);
	glUniform4f(loc, value1, value2, value3, value4);
}

void OpenGLShaderProgramBase::setUniformMat4(const char * uniform, float * value)
{
	GLuint loc = glGetUniformLocation(program, uniform);
	glUniformMatrix4fv(loc, 1, true, value);
}

void OpenGLShaderProgramBase::setUniformMat2(const char * uniform, float * value)
{
	GLuint loc = glGetUniformLocation(program, uniform);
	glUniformMatrix2fv(loc, 1, true, value);
}

unsigned OpenGLShaderProgramBase::getAttrLocation(const char * attr)
{
	return glGetAttribLocation(program, attr);
}

void OpenGLShaderProgramBase::setPatchVertexCount(unsigned int count)
{
	glPatchParameteri(GL_PATCH_VERTICES, count);
}
