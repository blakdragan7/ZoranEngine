#include "stdafx.h"
#include "TextureBase.h"
#include <ThirdParty/loadong/lodepng.h>
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
