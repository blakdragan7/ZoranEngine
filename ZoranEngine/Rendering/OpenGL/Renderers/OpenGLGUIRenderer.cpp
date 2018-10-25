#include "stdafx.h"
#include "OpenGLGUIRenderer.h"
#include <Rendering/RenderEngineBase.h>
#include <Rendering/OpenGL/OpenGLContext.h>
#include <Rendering/OpenGL/GUI/OpenGLGUIShader.h>
#include <Rendering/OpenGL/Renderers/OpenGLTriangleStripRenderer.h>

#include <Math/Matrix44.h>

OpenGLGUIRenderer::OpenGLGUIRenderer(OpenGLContext * context) : context(context)
{
	internalRenderer = context->CreateTriangleStripRenderer();
	internalRenderer->MakeFullScreenQuad();

	shader = (OpenGLGUIShader*)rEngine->CreateShaderProgram<OpenGLGUIShader>();
}


OpenGLGUIRenderer::~OpenGLGUIRenderer()
{
	delete internalRenderer;
}

void OpenGLGUIRenderer::RenderObject(const Matrix44 & cameraMatrix)
{
	shader->setUniformMat4("MVP", &cameraMatrix[0]);
	shader->setUniform("image", 0);

	internalRenderer->RenderObject(cameraMatrix);
}
