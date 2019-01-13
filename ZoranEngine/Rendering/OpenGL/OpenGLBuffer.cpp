#include "stdafx.h"
#include "OpenGLBuffer.h"
#include "OpenGLContext.h"

#include <GL/glew.h>

OpenGLBuffer * OpenGLBuffer::BufferFromVertexVector(const std::vector<Vector3D>& vector, OpenGLContext* context)
{
	if (vector.size() == 0)
	{
		Log(LogLevel_Error,"Trying to create an OpenGL Buffer from zero length vector !\n");
		return 0;
	}
	
	// TODO: test if simply using vector.ptr() will work instead of copying

	float* vecs = new float[vector.size() * 3];
	for (unsigned i = 0; i < vector.size(); i++)
	{
		vecs[i * 3] = vector[i].x;
		vecs[(i * 3) + 1] = vector[i].y;
		vecs[(i * 3) + 2] = vector[i].z;
	}

	OpenGLBuffer* buffer = new OpenGLBuffer(GL_ARRAY_BUFFER, vecs, 3, GL_DYNAMIC_DRAW, vector.size() * 3 * sizeof(float), context);
	delete[] vecs;

	return buffer;
}

OpenGLBuffer::OpenGLBuffer(OpenGLContext * context) :
	bufferSize(0), bufferType(0), usage(0), context(context)
{
	glGenBuffers(1, &glBuffer);
	context->CheckErrors("glGenBuffers");
}

OpenGLBuffer::OpenGLBuffer(unsigned bufferType, void* data, unsigned numComponents, unsigned usage, size_t size, OpenGLContext * context) :
	bufferSize(size), bufferType(bufferType), usage(usage), context(context)
{
	glGenBuffers(1, &glBuffer);
	context->CheckErrors("glGenBuffers");

	ReCreateBuffer(data, size, numComponents, usage);
}

OpenGLBuffer::~OpenGLBuffer()
{
	glDeleteBuffers(1, &glBuffer);
	context->CheckErrors("glDeleteBuffers");
}

void OpenGLBuffer::BindBuffer()
{
	glBindBuffer(bufferType, glBuffer);
	context->CheckErrors("glBindBuffer");
}

bool OpenGLBuffer::UpdateBuffer(void * data, size_t offset, size_t size)
{
	BindBuffer();
	if (size + offset <= bufferSize)
	{
		glBufferSubData(bufferType, offset, size, data);
		context->CheckErrors("glBufferSubData");
		return false;
	}
	else
	{
		bufferSize = size + offset;
		glBufferData(bufferType, bufferSize, data, usage);
		return true;
	}
}

void OpenGLBuffer::ReCreateBuffer(void * data, size_t size, unsigned numComponents, unsigned usage)
{
	BindBuffer();
	glBufferData(bufferType, size, data, usage);
	context->CheckErrors("glBufferData");

	this->usage = usage;
	this->bufferSize = size;
	this->numComponents = numComponents;
}

void OpenGLBuffer::ClearBuffer()
{
	GLenum format;

	switch (numComponents)
	{
	case 1:
		format = GL_R;
		break;
	case 2:
		format = GL_RG;
		break;
	case 3:
		format = GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		break;
	}

	BindBuffer();
	glClearBufferData(bufferType, format, format, GL_UNSIGNED_BYTE, 0);
	context->CheckErrors("ClearBuffer");
}

void OpenGLBuffer::ClearBuffer(size_t offset, size_t size)
{
	GLenum format;

	switch (numComponents)
	{
	case 1:
		format = GL_R;
		break;
	case 2:
		format = GL_RG;
		break;
	case 3:
		format = GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		break;
	}

	BindBuffer();
	glClearBufferSubData(bufferType, format, offset, size, format, GL_UNSIGNED_BYTE, 0);
	context->CheckErrors("ClearBuffer withOffset");
}
