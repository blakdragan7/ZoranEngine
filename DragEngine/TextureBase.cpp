#include "stdafx.h"
#include "TextureBase.h"
#include "lodepng.h"
#include <stdexcept>

unsigned TextureBase::LoadFromPNG(const char * path, unsigned & x, unsigned & y, unsigned char * data)
{
	return lodepng_decode_file(&data, &x, &y, path, LCT_RGBA,8);
}

TextureBase::TextureBase(RenderEngine * engine, RenderDataType type, RenderDataFormat format)
{
	if (engine == 0)throw std::invalid_argument("TextureBase::TextureBase() Render Engine can not be NULL!");
	this->engine = engine;
	this->type = type;
	this->format = format;

	containsData = false;
}

TextureBase::~TextureBase()
{
}
