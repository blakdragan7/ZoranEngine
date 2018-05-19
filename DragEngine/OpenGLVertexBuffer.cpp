#include "stdafx.h"
#include "OpenGLVertexBuffer.h"
#include "GL/glew.h"
#include <stdexcept>

OpenGLVertexBuffer::OpenGLVertexBuffer()
{
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
}


OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	glDeleteBuffers(1,&vbo);
	glDeleteVertexArrays(1, &vao);
}

void OpenGLVertexBuffer::CreateBufferFromMemory(VertexType vertType, DrawType drawType, unsigned numVerts, void* verts)
{
	this->numVerts = numVerts;

	this->drawType = drawType;
	this->vertType = vertType;

	this->glDrawType = GLDrawTypeFromDrawType(drawType);
	this->glVertType = GLVertexTypeFromVertexType(vertType);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*numVerts*3, verts, glDrawType);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, glVertType, GL_FALSE, 0, 0);
}

unsigned OpenGLVertexBuffer::GLDrawTypeFromDrawType(DrawType type)
{
	switch (type)
	{
	case DT_Dynamic:
		return GL_DYNAMIC_DRAW;
		break;
	case DT_Static:
		return GL_STATIC_DRAW;
		break;
	default:
		throw std::invalid_argument("OpenGLVertexBuffer::GLDrawTypeFromDrawType DrawType Not Recognized !!");
	}
}

unsigned OpenGLVertexBuffer::GLVertexTypeFromVertexType(VertexType type)
{
	switch (type)
	{
	case VT_Float:
		return GL_FLOAT;
		break;
	default:
		throw std::invalid_argument("OpenGLVertexBuffer::GLVertexTypeFromVertexType VertexType Not Recognized !!");
	}
}

void OpenGLVertexBuffer::BindBuffer()
{
	glBindVertexArray(vao);
}

void OpenGLVertexBuffer::UnbindBuffer()
{
	glBindVertexArray(0);
}

void OpenGLVertexBuffer::DrawArrays()
{
	glDrawArrays(drawType,0,numVerts);
}
