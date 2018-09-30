#include "stdafx.h"
#include "OpenGLFrameBuffer.h"

#include <Rendering/TextureBase.h>
#include <Rendering/OpenGL/OpenGLContext.h>
#include <GL/glew.h>

void OpenGLFrameBuffer::SetupOpenGL()
{
	glGenRenderbuffers(1, &renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.w, size.h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenBuffers(1, &pixelBuffer);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, pixelBuffer);
	glBufferData(GL_PIXEL_PACK_BUFFER, size.w * size.h, 0, GL_STREAM_READ);

	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		targetTexture->GetTextureID(),
		0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER,
		renderBuffer);

	// check FBO status
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		if (status == GL_FRAMEBUFFER_UNSUPPORTED)
		{
			Log(LogLevel_Error,"GL_FRAMEBUFFER_UNSUPPORTED");
		}
		Log(LogLevel_Error, "Error Creating Buffers Code: %i\n", status);
		context->CheckErrors("Frame Buffer Creation");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

OpenGLFrameBuffer::OpenGLFrameBuffer(Vector2I renderSize, RenderDataType renderType, RenderDataFormat format, OpenGLContext * context) : context(context), FrameBufferBase(renderSize,renderType,format)
{
	size = targetTexture->GetSize();
	SetupOpenGL();
}

OpenGLFrameBuffer::OpenGLFrameBuffer(TextureBase * targetTexture, OpenGLContext * context) : context(context), FrameBufferBase(targetTexture)
{
	size = targetTexture->GetSize();
	SetupOpenGL();
}

OpenGLFrameBuffer::~OpenGLFrameBuffer()
{
	glDeleteBuffers(1, &pixelBuffer);
	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteRenderbuffers(1, &renderBuffer);
}

void OpenGLFrameBuffer::RenderObject(const Matrix44 & cameraMatrix)
{
	GLint m_viewport[4];
	GLint previousFrameBuffer = 0;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &previousFrameBuffer);
	glGetIntegerv(GL_VIEWPORT, m_viewport);

	context->CheckErrors("glGetIntegerv");

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, size.w, size.h);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	FrameBufferBase::RenderObject(cameraMatrix);

	glBindFramebuffer(GL_FRAMEBUFFER, previousFrameBuffer);

	glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
}
