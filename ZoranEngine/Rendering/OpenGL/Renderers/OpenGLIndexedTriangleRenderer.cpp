#include "stdafx.h"
#include "OpenGLIndexedTriangleRenderer.h"
#include <Rendering/OpenGL/OpenGLVertexGroup.h>

#include <GL/glew.h>

#include <Rendering/OpenGL/OpenGLBuffer.h>

OpenGLIndexedTriangleRenderer::OpenGLIndexedTriangleRenderer(OpenGLContext* context) : context(context), IndexedTriangleRenderer(DT_Dynamic)
{
	vertexGroup = new OpenGLVertexGroup(GL_TRIANGLES, context);
}

OpenGLIndexedTriangleRenderer::~OpenGLIndexedTriangleRenderer()
{
	delete vertexGroup;
}

void OpenGLIndexedTriangleRenderer::RenderObject(const Matrix44 & cameraMatrix)
{
	vertexGroup->RenderObject();
}

void OpenGLIndexedTriangleRenderer::MakeFullScreenQuad()
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

void OpenGLIndexedTriangleRenderer::SetTriangles(const std::vector<TrianglePrimitive>& rectangles)
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

void OpenGLIndexedTriangleRenderer::SetIndecies(std::vector<unsigned>& indecies)
{
	OpenGLBuffer* ibo = new OpenGLBuffer(GL_ELEMENT_ARRAY_BUFFER, &indecies.at(0), 1, GL_DYNAMIC_DRAW, indecies.size() * sizeof(unsigned), context);

	vertexGroup->SetElementBuffer(ibo);
}
