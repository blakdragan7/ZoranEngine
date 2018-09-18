#include "stdafx.h"
#include "OpenGLRenderObject.h"
#include "GL/glew.h"
#include "OpenGLShaderProgramBase.h"
#include "OpenGLContext.h"

OpenGLRenderObject::OpenGLRenderObject(OpenGLContext* engine) : OGLContext(engine), RenderedObjectBase(OPENGL_IMPLEMENTATION)
{
	vbo = -1;
	tbo = -1;
	
	glGenVertexArrays(1, &vao);
	OGLContext->CheckErrors("OpenGLRenderObject()");
}


OpenGLRenderObject::~OpenGLRenderObject()
{
	if(vbo != -1)glDeleteBuffers(1, &vbo);
	if(tbo != -1)glDeleteBuffers(1, &tbo);
	glDeleteVertexArrays(1, &vao);

	if (cpuVertData)free(cpuVertData);
	if (cpuUVData)free(cpuUVData);
}

void OpenGLRenderObject::UpdateObjectFromMemory(unsigned numVerts, unsigned offset, void * verts, void * uv, bool copy)
{
	if (vbo != -1 && verts && numVerts > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(float)*numVerts * 3, verts);
	}
	if (tbo != -1 && uv && numVerts > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, tbo);
		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(float)*numVerts * 2, uv);
	}
}

void OpenGLRenderObject::CreateObjectFromMemory(PrimitiveType pType, VertexType vertType, DrawType drawType, unsigned numVerts, void* verts, void* uv, bool copy)
{
	this->numVerts = numVerts;

	this->drawType = drawType;
	this->vertType = vertType;

	this->glDrawType = GLPrimitveFromPrimitiveType(pType);
	this->glBufferDrawType = GLDrawTypeFromDrawType(drawType);
	this->glVertType = GLVertexTypeFromVertexType(vertType);

	if (verts)
	{
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*numVerts * 3, verts, glBufferDrawType);
	}
	if (uv)
	{
		glGenBuffers(1, &tbo);
		glBindBuffer(GL_ARRAY_BUFFER, tbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*numVerts * 2, uv, glBufferDrawType);
	}
	glBindVertexArray(vao);
	if (vbo != -1)
	{
		glEnableVertexAttribArray(vertLocation);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(vertLocation, 3, glVertType, GL_FALSE, 0, 0);
	}
	if (tbo != -1)
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
		if (tbo != -1)
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
	OGLContext->CheckErrors("CreateObjectFromMemory");
}

unsigned OpenGLRenderObject::GLDrawTypeFromDrawType(DrawType type)const
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

unsigned OpenGLRenderObject::GLVertexTypeFromVertexType(VertexType type)const
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

unsigned OpenGLRenderObject::GLPrimitveFromPrimitiveType(PrimitiveType type) const
{
	switch (type)
	{
	case PT_Square:
		return GL_QUADS;
	case PT_Triangle_Strip:
		return GL_TRIANGLE_STRIP;
	case PT_Triangle:
		return GL_TRIANGLES;
	case PT_Dot:
		return GL_POINTS;
	case PT_Lines:
		return GL_LINES;
	case PT_Line_Loop:
		return GL_LINE_LOOP;
	}

	return 0;
}

void OpenGLRenderObject::RenderObject()
{
	glBindVertexArray(vao);
	
	glDrawArrays(glDrawType,0,numVerts);

	OGLContext->CheckErrors("RenderObject");
}

void OpenGLRenderObject::MakeFullScreenQuad()
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

	static float vert[12] = { -1.0f,-1.0f, 0.0f,-1.0f,1.0f,0.0f,1.0f,-1.0f,0.0f,1.0f,1.0f,0.0f };
	static float coord[8] = { 0.0f,0.0f,0.0f,1.0f,1.0f,0.0f,1.0f,1.0f };

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

	OGLContext->CheckErrors("MakeFullScreenQuad");
}

void OpenGLRenderObject::SetAlphaEnabled(bool enabled)
{
	if (enabled)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		glDisable(GL_BLEND);
	}

	OGLContext->CheckErrors("SetAlphaEnabled");
}

bool OpenGLRenderObject::GetVertDataAsfloat(float ** data, unsigned & amount)
{
	switch (this->vertType)
	{
	case VT_Float:
	{
		float* floatData = new float[numVerts];
		for (unsigned i = 0; i < numVerts; i++)
		{
			floatData[i] = ((float*)cpuVertData)[i];
		}
		*data = floatData;
		amount = numVerts;
		return true;
	}
	}
	return false;
}
