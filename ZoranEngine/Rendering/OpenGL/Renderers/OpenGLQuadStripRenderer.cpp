#include "stdafx.h"
#include "OpenGLQuadStripRenderer.h"
#include <Rendering/OpenGL/OpenGLVertexGroup.h>
#include <GL/glew.h>
OpenGLQuadStripRenderer::OpenGLQuadStripRenderer(OpenGLContext* context) : context(context), QuadStripRenderer(DT_Dynamic)
{
	vertexGroup = new OpenGLVertexGroup(GL_QUAD_STRIP,context);
}


OpenGLQuadStripRenderer::~OpenGLQuadStripRenderer()
{
	delete vertexGroup;
}
