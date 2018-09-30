#include "stdafx.h"
#include "OpenGLTriangleStripRenderer.h"
#include <Rendering/OpenGL/OpenGLVertexGroup.h>
#include <Rendering/OpenGL/OpenGLContext.h>
#include <Rendering/OpenGL/OpenGLBuffer.h>

#include <GL/glew.h>

OpenGLTriangleStripRenderer::OpenGLTriangleStripRenderer(OpenGLContext* context) : context(context), TriangleStripRenderer(DT_Dynamic)
{
	vertexGroup = new OpenGLVertexGroup(GL_TRIANGLE_STRIP,context);
}

OpenGLTriangleStripRenderer::~OpenGLTriangleStripRenderer()
{
}

void OpenGLTriangleStripRenderer::MakeFullScreenQuad()
{
	static float vert[12] = { -1.0f,-1.0f, 0.0f,-1.0f,1.0f,0.0f,1.0f,-1.0f,0.0f,1.0f,1.0f,0.0f };
	static float coord[8] = { 0.0f,0.0f,0.0f,1.0f,1.0f,0.0f,1.0f,1.0f };

	OpenGLBuffer* vbo = new OpenGLBuffer(GL_ARRAY_BUFFER, vert, 3, GL_DYNAMIC_DRAW, sizeof(vert), context);
	OpenGLBuffer* tbo = new OpenGLBuffer(GL_ARRAY_BUFFER, coord, 2, GL_DYNAMIC_DRAW, sizeof(coord), context);

	// TODO: not hardcode these attr values
	vertexGroup->AddBufferForAttr(0, vbo);
	vertexGroup->AddBufferForAttr(1, tbo);

	vertexGroup->SetNumVerts(4);
}

void OpenGLTriangleStripRenderer::BeginTriangles(Vector3D ll, Vector3D tc, Vector3D lr)
{
	triangleBuilder.clear();
	triangleBuilder.push_back(ll);
	triangleBuilder.push_back(tc);
	triangleBuilder.push_back(lr);
}

void OpenGLTriangleStripRenderer::AddTriangleToStrip(Vector3D nextVertex)
{
	triangleBuilder.push_back(nextVertex);
}

void OpenGLTriangleStripRenderer::EndTriangles()
{
	OpenGLBuffer* buffer = OpenGLBuffer::BufferFromVertexVector(triangleBuilder,context);
	vertexGroup->AddBufferForAttr(0, buffer);
	triangleBuilder.clear();
}

void OpenGLTriangleStripRenderer::RenderObject(const Matrix44& cameraMatrix)
{
	vertexGroup->RenderObject();
}
