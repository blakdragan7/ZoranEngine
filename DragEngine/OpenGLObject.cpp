#include "stdafx.h"
#include "OpenGLObject.h"
#include "GL/glew.h"
#include <stdexcept>
#include "RenderEngineBase.h"

OpenGLObject::OpenGLObject()
{
	vbo = -1;
	tbo = -1;
	
	glGenVertexArrays(1, &vao);
	renderEngine->CheckErrors("OpenGLObject()");
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
	renderEngine->CheckErrors("CreateObjectFromMemory");
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
	
	glDrawArrays(glDrawType,0,numVerts);

	renderEngine->CheckErrors("RenderObject");
}

void OpenGLObject::MakeFullScreenQuad()
{
	this->numVerts = 4;

	this->drawType = DT_Dynamic;
	this->vertType = VT_Float;

	this->glDrawType = GL_TRIANGLE_STRIP;
	this->glBufferDrawType = GLDrawTypeFromDrawType(drawType);
	this->glVertType = GLVertexTypeFromVertexType(vertType);

	if (this->cpuVertData)
		delete[] this->cpuVertData;
	if (this->cpuUVData)
		delete[] this->cpuUVData;

	float vert[12] = { -1.0f,-1.0f, 0.0f,-1.0f,1.0f,0.0f,1.0f,-1.0f,0.0f,1.0f,1.0f,0.0f };
	float coord[8] = { 0.0f,0.0f,0.0f,1.0f,1.0f,0.0f,1.0f,1.0f };

	this->cpuVertData = malloc(sizeof(vert));
	this->cpuUVData = malloc(sizeof(coord));

	memcpy(this->cpuVertData, vert, sizeof(vert));
	memcpy(this->cpuUVData, coord, sizeof(coord));

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &tbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), this->cpuVertData, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(coord), this->cpuUVData, GL_DYNAMIC_DRAW);

	glBindVertexArray(vao);

	glEnableVertexAttribArray(vertLocation);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(vertLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);


	glEnableVertexAttribArray(UVLocation);
	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glVertexAttribPointer(UVLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

	renderEngine->CheckErrors("MakeFullScreenQuad");
}
