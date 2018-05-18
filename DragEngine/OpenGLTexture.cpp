#include "stdafx.h"
#include "OpenGLTexture.h"
#include "OpenGLRenderEngine.h"

#include "GL\glew.h"

OpenGLTexture::OpenGLTexture(RenderEngine * engine, RenderDataType type, RenderDataFormat format) : TextureBase(engine,type,format)
{
	OpenGLRenderEngine* oglEngine = dynamic_cast<OpenGLRenderEngine*>(engine);
	if (oglEngine == 0)throw std::invalid_argument("OpenGLTexture::OpenGLTexture() Must pass an OpenGL Engine to OpenGLTexture Class !");
}


OpenGLTexture::~OpenGLTexture()
{
}

void OpenGLTexture::LoadFromPath(const char * texture_path, RenderDataType type, RenderDataFormat format)
{
	unsigned x;
	unsigned y;
	unsigned char *data;
	unsigned error = LoadFromPNG(texture_path, x, y, data);
}

void OpenGLTexture::LoadFromMemory(unsigned x, unsigned y, void * data, RenderDataType type, RenderDataFormat format)
{
}

void OpenGLTexture::SetRenderDataType(RenderDataType newType)
{
}

void OpenGLTexture::SetRenderDataFormat(RenderDataFormat newFormat)
{
}
