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

	ReCreateBuiffer(data, size, numComponents, usage);
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

void OpenGLBuffer::UpdateBuffer(void * data, size_t offset, size_t size)
{
	if (size + offset < bufferSize - 1)
	{
		glBufferSubData(bufferType, offset, size, data);
		context->CheckErrors("glBufferSubData");
	}
	else
	{
		Log(LogLevel_Error, "Trying to Update GL Buffer with an offset + size greater then the buffer size!\n");
	}
}

void OpenGLBuffer::ReCreateBuiffer(void * data, size_t size, unsigned numComponents, unsigned usage)
{
	BindBuffer();
	glBufferData(bufferType, size, data, usage);
	context->CheckErrors("glBufferData");

	this->usage = usage;
	this->bufferSize = size;
	this->numComponents = numComponents;
}
