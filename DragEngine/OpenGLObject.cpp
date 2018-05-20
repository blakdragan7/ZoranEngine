#include "stdafx.h"
#include "OpenGLObject.h"
#include "GL/glew.h"
#include <stdexcept>

OpenGLObject::OpenGLObject()
{
	vbo = -1;
	tbo = -1;
	
	glGenVertexArrays(1, &vao);
}


OpenGLObject::~OpenGLObject()
{
	if(vbo != -1)glDeleteBuffers(1, &vbo);
	if(tbo != -1)glDeleteBuffers(1, &tbo);
	glDeleteVertexArrays(1, &vao);

	if (cpuVertData)free(cpuVertData);
	if (cpuUVData)free(cpuUVData);
}

void OpenGLObject::CreateObjectFromMemory(VertexType vertType, DrawType drawType, unsigned numVerts, void* verts, void* uv, bool copy)
{
	this->numVerts = numVerts;

	this->drawType = drawType;
	this->vertType = vertType;

	this->glDrawType = GL_TRIANGLES;
	this->glBufferDrawType = GLDrawTypeFromDrawType(drawType);
	this->glVertType = GLVertexTypeFromVertexType(vertType);

	if (vbo)
	{
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*numVerts * 3, verts, glBufferDrawType);
	}
	if (tbo)
	{
		glGenBuffers(1, &tbo);
		glBindBuffer(GL_ARRAY_BUFFER, tbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*numVerts * 2, uv, glBufferDrawType);
	}
	glBindVertexArray(vao);
	if (vbo)
	{
		glEnableVertexAttribArray(vertLocation);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(vertLocation, 3, glVertType, GL_FALSE, 0, 0);
	}
	if (tbo)
	{
		glEnableVertexAttribArray(UVLocation);
		glBindBuffer(GL_ARRAY_BUFFER, tbo);
		glVertexAttribPointer(UVLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}
	if (copy)
	{
		if (vbo)
		{
			cpuVertData = malloc(sizeof(float)*numVerts * 3);
			memcpy(cpuVertData, verts, sizeof(float)*numVerts * 3);
		}
		if (tbo)
		{
			cpuUVData = malloc(sizeof(float)*numVerts * 2);
			memcpy(cpuUVData, uv, sizeof(float)*numVerts * 2);
		}
	}
	else
	{
		cpuVertData = verts;
		cpuUVData = uv;
	}
}

unsigned OpenGLObject::GLDrawTypeFromDrawType(DrawType type)
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

unsigned OpenGLObject::GLVertexTypeFromVertexType(VertexType type)
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

void OpenGLObject::RenderObject()
{
	glBindVertexArray(vao);
	glDrawArrays(drawType,0,numVerts);
}
