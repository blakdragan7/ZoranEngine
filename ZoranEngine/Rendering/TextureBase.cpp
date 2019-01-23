#include "stdafx.h"
#include "TextureBase.h"
#include <ThirdParty/loadpng/lodepng.h>
#include <ThirdParty/stb/stb_image.h>
#include <stdexcept>

unsigned TextureBase::LoadFromPNG(const char * path, unsigned & x, unsigned & y, unsigned char ** data)
{
	return lodepng_decode_file(data, &x, &y, path, LCT_RGBA,8);
}

TextureBase::TextureBase(unsigned width_, unsigned height_,RenderDataType type, RenderDataFormat format) : 
	type(type), format(format), width(width_), height(height_), containsData(false)
{
}

TextureBase::~TextureBase()
{
}

int TextureBase::GetNumComponents()const
{
	switch (type)
	{
	case Render_Data_Type_R_8:
		return 1;
	case Render_Data_Type_RG_16:
	case Render_Data_Type_RA_16:
		return 2;
	case Render_Data_Type_RGB_24:
		return 3;
	case Render_Data_Type_RGBA_32:
	case Render_Data_Type_BGRA_32:
		return 4;
	default:
		return 1;
	}
}

int TextureBase::GetComponentSize()const
{
	switch (format)
	{
	case Render_Data_Format_Byte:
	case Render_Data_Format_Unsigned_Byte:
		return sizeof(char);
	case Render_Data_Format_Float:
		return sizeof(float);
	default:
		return 1;
	}
}

int TextureBase::LoadFromPath(const char * texture_path, RenderDataType type, RenderDataFormat format)
{
	int w, h, c;

	auto image = stbi_load(texture_path, &w, &h, &c, 0);

	RenderDataType sourceType;

	if (image)
	{
		switch (c)
		{
		case 1:
			sourceType = Render_Data_Type_R_8;
			break;
		case 2:
			sourceType = Render_Data_Type_RA_16;
			break;
		case 3:
			sourceType = Render_Data_Type_RGB_24;
			break;
		case 4:
			sourceType = Render_Data_Type_RGBA_32;
			break;
		}

		LoadFromMemory(w, h, image, sourceType, format);

		stbi_image_free(image);
		
		return 0;
	}
	else
	{
		Log(LogLevel_Error, "failed to load texture at path %s with error %s!", texture_path, stbi_failure_reason());
		return -1;
	}
}

