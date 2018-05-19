#include "stdafx.h"
#include "OpenGLTexture.h"
#include "OpenGLRenderEngine.h"

#include "GL\glew.h"

OpenGLTexture::OpenGLTexture(RenderEngineBase * engine, unsigned width_, unsigned height_, RenderDataType type_, RenderDataFormat format_) : TextureBase(engine,width_,height_,type_,format_)
{
	OpenGLRenderEngine* oglEngine = dynamic_cast<OpenGLRenderEngine*>(engine);
	if (oglEngine == 0)throw std::invalid_argument("OpenGLTexture::OpenGLTexture() Must pass an OpenGL Engine to OpenGLTexture Class !");

	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &gl_texture);
}


OpenGLTexture::~OpenGLTexture()
{
	glDeleteTextures(1, &gl_texture);
}

void OpenGLTexture::BindTexture(unsigned textureNumber)
{
	glActiveTexture(GL_TEXTURE0 + textureNumber);
	glBindTexture(GL_TEXTURE_2D, gl_texture);
}

void OpenGLTexture::UnbindTexture(unsigned textureNumber)
{
	glActiveTexture(GL_TEXTURE0 + textureNumber);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLTexture::LoadFromPath(const char * texture_path, RenderDataType type, RenderDataFormat format)
{
	unsigned w = 0;
	unsigned h = 0;
	unsigned char *data = 0;
	unsigned error = LoadFromPNG(texture_path, w, h, data);

	LoadFromMemory(w, h, data, RenderDataType::TYPE_RGBA_32);
}

void OpenGLTexture::LoadFromMemory(unsigned w, unsigned h, void * data, RenderDataType type, RenderDataFormat format)
{
	glBindTexture(GL_TEXTURE_2D, gl_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GLFormatFromRenderDataFormat(this->format), width, height, 0, GLFormatFromRenderDataFormat(format), GLTypeFromRenderDataType(type), data);

	glBindTexture(GL_TEXTURE_2D, 0);

}

void OpenGLTexture::SetRenderDataType(RenderDataType newType)
{
}

void OpenGLTexture::SetRenderDataFormat(RenderDataFormat newFormat)
{
}

unsigned OpenGLTexture::GLTypeFromRenderDataType(RenderDataType type)
{
	switch (type)
	{
	case TYPE_BGRA_32:
		return GL_BGRA;
		break;
	case TYPE_RGBA_32:
		return GL_RGBA;
		break;
	case TYPE_RGB_24:
		return GL_RGB;
		break;
	case TYPE_RG_16:
		return GL_RG;
		break;
	case TYPE_R_8:
		return GL_R;
		break;
	default:
		throw std::invalid_argument("OpenGLTexture::GLTypeFromRenderDataType RenderDataType Not Recognized !");
		break;
	}
}

unsigned OpenGLTexture::GLFormatFromRenderDataFormat(RenderDataFormat format)
{
	switch (format)
	{
	case FORMAT_BYTE:
		return GL_BYTE;
		break;
	case FORMAT_UNSIGNED_BYTE:
		return GL_UNSIGNED_BYTE;
	case FORMAT_FLOAT:
		return GL_FLOAT;
		break;
	default:
		throw std::invalid_argument("OpenGLTexture::GLFormatFromRenderDataFormat RenderDataFormat Not Recognized !!");
		break;
	}
}