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

void OpenGLTriangleStripRenderer::AddRectanglesToStrip(const std::vector<RectanglePrimitive>& rectangles)
{
	size_t size = rectangles.size() * 4;

	float* verts = new float[size * 3];
	float* coords = new float[size * 2];
	for (size_t i = 0; i < rectangles.size(); i++)
	{
		//Log(LogLevel_Error, "before x %F y %f z %f\n", rectangles[i].vertecies[1].x, rectangles[i].vertecies[1].y, rectangles[i].vertecies[1].z);

		memcpy(&verts[i * 4 * 3], &rectangles[i].vertecies, sizeof(float) * 4 * 3);
		memcpy(&coords[i * 4 * 2], &rectangles[i].coords, sizeof(float) * 4 * 2);

		//Log(LogLevel_Error, "after x %F y %f z %f\n", verts[i * 4 * 3]+3, verts[(i * 4 * 3) + 4], verts[(i * 4 * 3) + 5]);
	}

	OpenGLBuffer* vbo = new OpenGLBuffer(GL_ARRAY_BUFFER, verts, 3, GL_DYNAMIC_DRAW, size * 3 * sizeof(float), context);
	OpenGLBuffer* tbo = new OpenGLBuffer(GL_ARRAY_BUFFER, coords, 2, GL_DYNAMIC_DRAW, size * 2 * sizeof(float), context);

	delete[] verts;
	delete[] coords;

	vertexGroup->AddBufferForAttr(0, vbo);
	vertexGroup->AddBufferForAttr(1, tbo);

	vertexGroup->SetNumVerts(static_cast<int>(size));
}
