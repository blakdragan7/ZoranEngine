#include "stdafx.h"
#include "OpenGLGUIColoredRectRenderer.h"

#include <Rendering/OpenGL/OpenGLBuffer.h>
#include <Rendering/OpenGL/OpenGLVertexGroup.h>
#include <Rendering/OpenGL/GUI/OpenGLGUIColoredRectShader.h>
#include <Rendering/RenderEngineBase.h>
#include <GL/glew.h>

OpenGLGUIColoredRectRenderer::OpenGLGUIColoredRectRenderer(OpenGLContext* context) : context(context)
{
	vertGroup = new OpenGLVertexGroup(GL_TRIANGLE_STRIP, context);
	shader = (OpenGLGUIColoredRectShader*)rEngine->CreateShaderProgram<OpenGLGUIColoredRectShader>();

	static float vert[12] = { -1.0f,-1.0f, 0.0f,-1.0f,1.0f,0.0f,1.0f,-1.0f,0.0f,1.0f,1.0f,0.0f };

	OpenGLBuffer* vbo = new OpenGLBuffer(GL_ARRAY_BUFFER, vert, 3, GL_DYNAMIC_DRAW, sizeof(vert), context);

	// TODO: not hardcode these attr values
	vertGroup->AddBufferForAttr(0, vbo);

	vertGroup->SetNumVerts(4);
}

OpenGLGUIColoredRectRenderer::~OpenGLGUIColoredRectRenderer()
{
	delete vertGroup;
}

void OpenGLGUIColoredRectRenderer::SetLinearBlend()
{
	static float coord[16] = {	
								1.0f,1.0f,1.0f,1.0f, // bottomleft
								0.0f,0.0f,0.0f,0.0f, // top left
								1.0f,1.0f,1.0f,1.0f, // bottom right
								0.0f,0.0f,0.0f,0.0f	 // top right
							};

	OpenGLBuffer* abo = new OpenGLBuffer(GL_ARRAY_BUFFER, coord, 4, GL_DYNAMIC_DRAW, sizeof(coord), context);
	vertGroup->AddBufferForAttr(1, abo);
}

void OpenGLGUIColoredRectRenderer::RenderObject(const Matrix44 & cameraMatrix)
{
	shader->BindProgram();
	shader->setUniformMat4("MVP", &cameraMatrix(0, 0));
	shader->setUniform("ColorA", &ColorA);
	shader->setUniform("ColorB", &ColorB);

	vertGroup->RenderObject();
}
