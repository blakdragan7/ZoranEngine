#include "stdafx.h"
#include "OpenGLVertexArray.h"
#include "OpenGLContext.h"
#include "OpenGLBuffer.h"
#include <GL/glew.h>

unsigned OpenGLVertexArray::EnableVAO = -1;

OpenGLVertexArray::OpenGLVertexArray(OpenGLContext * context)
{
	glGenVertexArrays(1, &vao);
	context->CheckErrors("glGenVertexArrays");
}

OpenGLVertexArray::~OpenGLVertexArray()
{
	glDeleteVertexArrays(1, &vao);
	context->CheckErrors("glGenVertexArrays");
}

void OpenGLVertexArray::BindBufferForAttr(unsigned attr, OpenGLBuffer * buffer)
{
	if(EnableVAO != vao)
		EnableArray();
	
	glEnableVertexAttribArray(attr);
	context->CheckErrors("glGenVertexArrays");
	buffer->BindBuffer();
	glVertexAttribPointer(attr, buffer->GetNumComponents(), GL_FLOAT, GL_FALSE, 0, 0);
	context->CheckErrors("glGenVertexArrays");
}

void OpenGLVertexArray::EnableArray()
{
	glBindVertexArray(vao);
	if(context->CheckErrors("glGenVertexArrays") == false)
		EnableVAO = vao;
}
