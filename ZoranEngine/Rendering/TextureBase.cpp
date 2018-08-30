#include "stdafx.h"
#include "TextureBase.h"
#include <ThirdParty/loadong/lodepng.h>
#include <stdexcept>

unsigned TextureBase::LoadFromPNG(const char * path, unsigned & x, unsigned & y, unsigned char ** data)
{
	return lodepng_decode_file(data, &x, &y, path, LCT_RGBA,8);
}

TextureBase::TextureBase(RenderEngineBase * engine,unsigned width_, unsigned height_,RenderDataType type, RenderDataFormat format) : 
	engine(engine), type(type), format(format), width(width_), height(height_), containsData(false)
{
	if (engine == 0)throw std::invalid_argument("TextureBase::TextureBase() Render Engine can not be NULL!");
}

TextureBase::~TextureBase()
{
}
