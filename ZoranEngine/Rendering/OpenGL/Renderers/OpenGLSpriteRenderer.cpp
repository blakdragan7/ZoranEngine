#include "stdafx.h"
#include "OpenGLSpriteRenderer.h"
#include "OpenGLTriangleStripRenderer.h"
#include <Rendering/OpenGL/OpenGLContext.h>

OpenGLSpriteRenderer::OpenGLSpriteRenderer(OpenGLContext* context) : context(context), SpriteRenderer(PT_Unused,DT_Dynamic)
{
	innerRenderer = new OpenGLTriangleStripRenderer(context);

	innerRenderer->MakeFullScreenQuad();
}

OpenGLSpriteRenderer::~OpenGLSpriteRenderer()
{
	delete innerRenderer;
}

void OpenGLSpriteRenderer::RenderObject()
{
	innerRenderer->RenderObject();
}
