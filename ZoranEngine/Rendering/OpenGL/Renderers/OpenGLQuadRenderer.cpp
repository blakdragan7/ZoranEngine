#include "stdafx.h"
#include "OpenGLQuadRenderer.h"
#include <Rendering/OpenGL/OpenGLVertexGroup.h>
#include <Rendering/OpenGL/OpenGLContext.h>
#include <GL/glew.h>

OpenGLQuadRenderer::OpenGLQuadRenderer(OpenGLContext* context) : context(context) , QuadRenderer(DT_Dynamic)
{
	vertexGroup = new OpenGLVertexGroup(GL_QUADS,context);
}

OpenGLQuadRenderer::~OpenGLQuadRenderer()
{
	delete vertexGroup;
}
