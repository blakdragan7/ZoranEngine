#pragma once
#include <Math/Vector2.h>
#include <ThirdParty/msdfgen/msdfgen.h>

template<typename t>
void Insert(msdfgen::Bitmap<t>& dest,const msdfgen::Bitmap<t> &source, Vec2I offset)
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

static float* PixDataForBitmap(msdfgen::Bitmap<msdfgen::FloatRGB>& source)
{
	float* pixData = static_cast<float*>(malloc(source.width() * source.height() * 3 * sizeof(float)));

	memcpy(pixData,&source(0,0), source.width() * source.height() * 3 * sizeof(float));

	return pixData;
}

static msdfgen::Bitmap<msdfgen::FloatRGB>* BitmapFromMemory(unsigned char* bytes, int sizex,int sizey)
{
	msdfgen::Bitmap<msdfgen::FloatRGB> *bitmap = new msdfgen::Bitmap<msdfgen::FloatRGB>(sizex, sizey);

	for (int x = 0; x < sizex; x++)
	{
		for (int y = 0; y < sizey; y++)
		{
			int index = ((sizex * y) + x) * 3;

			float r, g, b;
			r = (float)bytes[index + 0] / (float)0x100;
			g = (float)bytes[index + 1] / (float)0x100;
			b = (float)bytes[index + 2] / (float)0x100;

			(*bitmap)(x, y).r = r;
			(*bitmap)(x, y).g = g;
			(*bitmap)(x, y).b = b;
		}
	}

	return bitmap;
}