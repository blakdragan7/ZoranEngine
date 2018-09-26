#include "stdafx.h"
#include "OpenGLTriangleRenderer.h"
#include <Rendering/OpenGL/OpenGLVertexGroup.h>

#include <GL/glew.h>

OpenGLTriangleRenderer::OpenGLTriangleRenderer(OpenGLContext* context) : context(context), TriangleRenderer(DT_Dynamic)
{
	vertexGroup = new OpenGLVertexGroup(GL_QUAD_STRIP, context);
}

OpenGLTriangleRenderer::~OpenGLTriangleRenderer()
{
	delete vertexGroup;
}
