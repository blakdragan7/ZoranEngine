#include "stdafx.h"
#include "OpenGLTexture.h"
#include "OpenGLContext.h"

#include "GL\glew.h"

#include <Utils/StringAddons.hpp>
#include <ThirdParty/loadbmp/loadbmp.hpp>

OpenGLTexture::OpenGLTexture(OpenGLContext* OGL, RenderDataType type_, RenderDataFormat format_) : OGL(OGL), TextureBase(0,0,type_,format_)
{
	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &gl_texture);
	OGL->CheckErrors("OpenGLTexture");
}


OpenGLTexture::~OpenGLTexture()
{
	glDeleteTextures(1, &gl_texture);
	OGL->CheckErrors("~OpenGLTexture");
}

void OpenGLTexture::BindTexture(unsigned textureNumber)const
{
	glActiveTexture(GL_TEXTURE0 + textureNumber);
	glBindTexture(GL_TEXTURE_2D, gl_texture);
	OGL->CheckErrors("BindTexture");
}

void OpenGLTexture::UnbindTexture(unsigned textureNumber)const
{
	glActiveTexture(GL_TEXTURE0 + textureNumber);
	glBindTexture(GL_TEXTURE_2D, 0);
	OGL->CheckErrors("UnbindTexture");
}

void OpenGLTexture::LoadFromPath(const char * texture_path, RenderDataType type, RenderDataFormat format)
{
	std::string fileType = StringToLower(GetStringFileType(texture_path));

	if (fileType == "png")
	{
		unsigned w = 0;
		unsigned h = 0;
		unsigned char *data = 0;

		unsigned error = LoadFromPNG(texture_path, w, h, &data);

		if (error)
		{
			Log(LogLevel_Error, "Could not load Texture %s error %i", texture_path, error);
		}

		width = w;
		height = h;

		LoadFromMemory(w, h, data, RenderDataType::Render_Data_Type_RGBA_32);
		free(data);
	}
	else if (fileType == "bmp")
	{
		unsigned w = 0;
		unsigned h = 0;
		unsigned comp = 3;
		unsigned char *data = 0;

		unsigned error = loadbmp_decode_file(texture_path, &data, &w, &h, comp);

		if (error)
		{
			Log(LogLevel_Error, "Could not load Texture %s error %i", texture_path, error);
			return;
		}

		LoadFromMemory(w, h, data, RenderDataType::Render_Data_Type_RGB_24);
		free(data);
	}
	else
	{
		Log(LogLevel_Error,"Incorrect File Type For Texture %s!", texture_path);
		return;
	}
}

void OpenGLTexture::LoadFromMemory(unsigned w, unsigned h, void * data, RenderDataType type, RenderDataFormat format)
{
	width = w;
	height = h;

	glBindTexture(GL_TEXTURE_2D, gl_texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GLTypeFromRenderDataType(this->type), width, height, 0, GLTypeFromRenderDataType(type), GLFormatFromRenderDataFormat(format), data);

	OGL->CheckErrors("LoadFromMemory");
}

void OpenGLTexture::SetRenderDataType(RenderDataType newType)
{
	OGL->CheckErrors("SetRenderDataType");
}

void OpenGLTexture::SetRenderDataFormat(RenderDataFormat newFormat)
{
	OGL->CheckErrors("SetRenderDataFormat");
}

void OpenGLTexture::UseTexture(unsigned long data)const
{
#pragma warning(push)
#pragma warning(disable:4302)
	BindTexture((unsigned)data);
#pragma warning(pop)
}

void OpenGLTexture::StopUsingTexture(unsigned long data)const
{
#pragma warning(push)
#pragma warning(disable:4311)
	UnbindTexture((unsigned)data);
#pragma warning(pop)
}

unsigned OpenGLTexture::GetTextureID()
{
	return gl_texture;
}

unsigned OpenGLTexture::GLTypeFromRenderDataType(RenderDataType type)
{
	switch (type)
	{
	case Render_Data_Type_BGRA_32:
		return GL_BGRA;
		break;
	case Render_Data_Type_RGBA_32:
		return GL_RGBA;
		break;
	case Render_Data_Type_RGB_24:
		return GL_RGB;
		break;
	case Render_Data_Type_RG_16:
		return GL_RG;
		break;
	case Render_Data_Type_R_8:
		return GL_RED;
		break;
	default:
		Log(LogLevel_Error, "OpenGLTexture::GLTypeFromRenderDataType RenderDataType Not Recognized !");
		return Render_Data_Type_BGRA_32;
		break;
	}
}

unsigned OpenGLTexture::GLFormatFromRenderDataFormat(RenderDataFormat format)
{
	switch (format)
	{
	case Render_Data_Format_Byte:
		return GL_BYTE;
		break;
	case Render_Data_Format_Unsigned_Byte:
		return GL_UNSIGNED_BYTE;
		break;
	case Render_Data_Format_Float:
		return GL_FLOAT;
		break;
	default:
		Log(LogLevel_Error,"OpenGLTexture::GLFormatFromRenderDataFormat RenderDataFormat Not Recognized !!");
		return Render_Data_Format_Unsigned_Byte;
		break;
	}
}