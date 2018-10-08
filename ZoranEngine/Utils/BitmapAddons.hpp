#pragma once
#include <Math/Vector2.h>
#include <ThirdParty/msdfgen/msdfgen.h>

template<typename t>
static void Insert(msdfgen::Bitmap<t>& dest,const msdfgen::Bitmap<t> &source, Vec2I offset)
{
	if (dest.height() < (offset.y + source.height()) || dest.width() < (offset.x + source.width()))
	{
		Log(LogLevel_Error,"Can not insert Into BMP, dest size is smaller then source + offset\n");
		return;
	}

	for (int y = 0; y < source.height(); y++)
	{
		for (int x = 0; x < source.width(); x++)
		{
			dest(offset.x + x, offset.y + y) = source(x,y);
		}
	}
}