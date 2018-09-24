#include "stdafx.h"
#include "OpenGLVertexGroup.h"
#include "GL/glew.h"
#include "OpenGLShaderProgramBase.h"
#include "OpenGLContext.h"
#include "OpenGLVertexArray.h"
#include "OpenGLBuffer.h"

OpenGLVertexGroup::OpenGLVertexGroup(unsigned vertexDrawType, OpenGLContext * context) :
	numVerts(0), vertexDrawType(vertexDrawType), OGLContext(context)
{
	vao = new OpenGLVertexArray(context);
}

OpenGLVertexGroup::OpenGLVertexGroup(unsigned vertexDrawType, unsigned numVertecies, OpenGLContext* context) :
	numVerts(numVertecies), vertexDrawType(vertexDrawType), OGLContext(context)
{
	vao = new OpenGLVertexArray(context);
}

OpenGLVertexGroup::~OpenGLVertexGroup()
{
	delete vao;
	for (auto& iter : bufferMap)
	{
		delete iter.second;
	}
	bufferMap.clear();
}

void OpenGLVertexGroup::RenderObject()
{
	if (numVerts == 0)
	{
		Log(LogLevel_Warning, "Trying To Draw 0 Verts in Vertex Group !\n");
	}

	vao->EnableArray();
	
	glDrawArrays(vertexDrawType,0,numVerts);

	OGLContext->CheckErrors("RenderObject");
}

void OpenGLVertexGroup::AddBufferForAttr(unsigned attr, unsigned type, void * data, unsigned numComponents, size_t size, unsigned usage)
{
	OpenGLBuffer* buffer = new OpenGLBuffer(type, data, numComponents, usage, size, OGLContext);
	bufferMap.insert({ attr, buffer });
	vao->BindBufferForAttr(attr, buffer);
	numVerts = size / numComponents;
}

void OpenGLVertexGroup::AddBufferForAttr(unsigned attr, OpenGLBuffer * buffer)
{
 	bufferMap.insert({ attr,buffer });
	vao->BindBufferForAttr(attr, buffer);
}

OpenGLBuffer * OpenGLVertexGroup::GetBufferForAttr(unsigned attr)const
{
	auto iter = bufferMap.find(attr);
	if (iter != bufferMap.end())
	{
		return iter->second;
	}
	Log(LogLevel_Error,"Trying To Access GL Named Buffer that wasn't added !!\n");
	return nullptr;
}
