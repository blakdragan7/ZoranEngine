#include "stdafx.h"
#include "OpenGLVertexGroup.h"
#include "GL/glew.h"
#include "OpenGLShaderProgramBase.h"
#include "OpenGLContext.h"
#include "OpenGLVertexArray.h"
#include "OpenGLBuffer.h"

OpenGLVertexGroup::OpenGLVertexGroup(unsigned vertexDrawType, OpenGLContext * context) :
	numVerts(0), vertexDrawType(vertexDrawType), OGLContext(context), hasIndecies(false)
{
	vao = new OpenGLVertexArray(context);
	bufferMap.set_empty_key(-1);
}

OpenGLVertexGroup::OpenGLVertexGroup(unsigned vertexDrawType, unsigned numVertecies, OpenGLContext* context) :
	numVerts(numVertecies), vertexDrawType(vertexDrawType), OGLContext(context), hasIndecies(false)
{
	vao = new OpenGLVertexArray(context);
	bufferMap.set_empty_key(-1);
}

OpenGLVertexGroup::~OpenGLVertexGroup()
{
	delete vao;
	for (auto& iter : bufferMap)
	{
		delete iter.second;
	}
	bufferMap.clear();

	if (indexBuffer)
	{
		delete indexBuffer;
	}
}

void OpenGLVertexGroup::RenderObject()
{
	if (numVerts == 0)
	{
		Log(LogLevel_Warning, "Trying To Draw 0 Verts in Vertex Group !\n");
	}

	vao->EnableArray();
	if (hasIndecies)
	{
		indexBuffer->BindBuffer();
		glDrawElements(vertexDrawType, numVerts, GL_UNSIGNED_INT, 0);
	}
	else
		glDrawArrays(vertexDrawType,0,numVerts);

	OGLContext->CheckErrors("RenderObject");
}

void OpenGLVertexGroup::AddBufferForAttr(unsigned attr, unsigned type, void * data, unsigned numComponents, size_t size, unsigned usage)
{
	OpenGLBuffer* buffer = new OpenGLBuffer(type, data, numComponents, usage, size, OGLContext);
	AddBufferForAttr(attr, buffer);
}

void OpenGLVertexGroup::AddBufferForAttr(unsigned attr, OpenGLBuffer * buffer)
{
	auto &itr = bufferMap.find(attr);
	if (itr != bufferMap.end())
	{
		delete itr->second;
		itr->second = buffer;
	}
	else
	{
		bufferMap.insert({ attr,buffer });
	}

	vao->BindBufferForAttr(attr, buffer);
}

void OpenGLVertexGroup::AddBufferForAttrNoCheck(unsigned attr, OpenGLBuffer * buffer)
{
	vao->BindBufferForAttr(attr, buffer);
}

void OpenGLVertexGroup::SetElementBuffer(OpenGLBuffer * buffer)
{
	if (indexBuffer)
	{
		delete indexBuffer;
	}
	indexBuffer = buffer;
	hasIndecies = indexBuffer != 0;
}

OpenGLBuffer * OpenGLVertexGroup::GetBufferForAttr(unsigned attr)const
{
	auto iter = bufferMap.find(attr);
	if (iter != bufferMap.end())
	{
		return iter->second;
	}
	Log(LogLevel_Warning,"Trying To Access GL Buffer From Vertex Group that wasn't added !!\n");
	return nullptr;
}
