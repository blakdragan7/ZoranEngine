#include "stdafx.h"
#include "OpenGLTriangleRenderer.h"
#include <Rendering/OpenGL/OpenGLVertexGroup.h>

#include <GL/glew.h>

#include <Rendering/OpenGL/OpenGLBuffer.h>

OpenGLTriangleRenderer::OpenGLTriangleRenderer(OpenGLContext* context) : context(context), TriangleRenderer(DT_Dynamic)
{
	vertexGroup = new OpenGLVertexGroup(GL_TRIANGLES, context);
}

OpenGLTriangleRenderer::~OpenGLTriangleRenderer()
{
	delete vertexGroup;
}

void OpenGLTriangleRenderer::RenderObject(const Matrix44 & cameraMatrix)
{
	vertexGroup->RenderObject();
}

void OpenGLTriangleRenderer::MakeFullScreenQuad()
{
	static float vert[18] = { 
		-1.0f,1.0f, 0.0f, // tl
		-1.0f,-1.0f,0.0f, // bl
		1.0f,-1.0f,0.0f, // br

		1.0f,1.0f,0.0f, // tr
		1.0f,-1.0f,0.0f, // br
		-1.0f,1.0f,0.0f }; // tl

	static float coord[12] = {
		0.0f,1.0f,
		0.0f,0.0f,
		1.0f,0.0f,

		1.0f,1.0f,
		1.0f,0.0f,
		0.0f,1.0f };

	OpenGLBuffer* vbo = new OpenGLBuffer(GL_ARRAY_BUFFER, vert, 3, GL_DYNAMIC_DRAW, sizeof(vert), context);
	OpenGLBuffer* tbo = new OpenGLBuffer(GL_ARRAY_BUFFER, coord, 2, GL_DYNAMIC_DRAW, sizeof(coord), context);

	// TODO: not hardcode these attr values
	vertexGroup->AddBufferForAttr(0, vbo);
	vertexGroup->AddBufferForAttr(1, tbo);

	vertexGroup->SetNumVerts(6);
}

void OpenGLTriangleRenderer::AddTriangles(const std::vector<float>& verts, std::vector<float>& uvs)
{
	OpenGLBuffer* vbo = new OpenGLBuffer(GL_ARRAY_BUFFER, (void*)&verts.at(0), 3, GL_DYNAMIC_DRAW, verts.size() * sizeof(float), context);
	OpenGLBuffer* tbo = new OpenGLBuffer(GL_ARRAY_BUFFER, (void*)&uvs.at(0), 2, GL_DYNAMIC_DRAW, uvs.size() * sizeof(float), context);

	vertexGroup->AddBufferForAttr(0, vbo);
	vertexGroup->AddBufferForAttr(1, tbo);

	vertexGroup->SetNumVerts(static_cast<int>(verts.size()));
}

void OpenGLTriangleRenderer::AddTriangles(float * verts, size_t vertSize, float * uvs, size_t uvSize)
{
	if (OpenGLBuffer* vbo = vertexGroup->GetBufferForAttr(0))
	{
		vbo->UpdateBuffer(verts, 0, vertSize * sizeof(float));

		/*if (vbo->UpdateBuffer(verts, 0, vertSize * sizeof(float)))
		{
			vertexGroup->AddBufferForAttrNoCheck(0, vbo);
		}*/
	}
	else
	{
		vbo = new OpenGLBuffer(GL_ARRAY_BUFFER, verts, 3, GL_DYNAMIC_DRAW, vertSize * sizeof(float), context);
		vertexGroup->AddBufferForAttr(0, vbo);
	}
	if (OpenGLBuffer* tbo = vertexGroup->GetBufferForAttr(1))
	{
		tbo->UpdateBuffer(uvs, 0, uvSize * sizeof(float));
		/*if (tbo->UpdateBuffer(uvs, 0, uvSize * sizeof(float)))
		{
			vertexGroup->AddBufferForAttr(1, tbo);
		}*/
	}
	else
	{
		tbo = new OpenGLBuffer(GL_ARRAY_BUFFER, uvs, 2, GL_DYNAMIC_DRAW, uvSize * sizeof(float), context);
		vertexGroup->AddBufferForAttr(1, tbo);
	}

	vertexGroup->SetNumVerts(static_cast<int>(vertSize / 3));

	// REALLY BAD TODO: figure out how to defer delete arrays passed in till they are done uploading
	glFinish();
}

void OpenGLTriangleRenderer::AddTriangles(const std::vector<TrianglePrimitive>& rectangles)
{
	size_t s = rectangles.size();
	size_t size = s * 3;

	float* verts = new float[size * 3];
	float* coords = new float[size * 2];
	for (size_t i = 0; i < s; i++)
	{
		memcpy(&verts[i * 3 * 3], &rectangles[i].vertecies, sizeof(float) * 3 * 3);
		memcpy(&coords[i * 3 * 2], &rectangles[i].coords, sizeof(float) * 3 * 2);
	}

	OpenGLBuffer* vbo = new OpenGLBuffer(GL_ARRAY_BUFFER, verts, 3, GL_DYNAMIC_DRAW, size * 3 * sizeof(float), context);
	OpenGLBuffer* tbo = new OpenGLBuffer(GL_ARRAY_BUFFER, coords, 2, GL_DYNAMIC_DRAW, size * 2 * sizeof(float), context);

	delete[] verts;
	delete[] coords;

	vertexGroup->AddBufferForAttr(0, vbo);
	vertexGroup->AddBufferForAttr(1, tbo);

	vertexGroup->SetNumVerts(static_cast<int>(size));
}
