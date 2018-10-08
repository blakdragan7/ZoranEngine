#include "stdafx.h"
#include "FontResource.h"

#include <ThirdParty/msdfgen/msdfgen.h>
#include <ThirdParty/msdfgen/msdfgen-ext.h>

#include <Rendering/TextureBase.h>
#include <Utils/StringAddons.hpp>
#include <Utils/BitmapAddons.hpp>

#include "ResourceErrors.h"

using namespace msdfgen;
template<typename t>
struct BMPLoadDataPart
{
	Bitmap<t> *bmp;
	Vector2I offset;
	uint32_t glyph;
};

class FontResourceInternal
{
private:
	FreetypeHandle* ft;
	FontHandle *font;
	Bitmap<FloatRGB>* bitmapData;
	Vector2I lastOffset;

	FontResourceInternal() : bitmapData(0), ft(0), font(0)
	{}
	~FontResourceInternal() 
	{
		if (font)destroyFont(font);
		if (ft)deinitializeFreetype(ft);
		if (bitmapData)delete bitmapData; 
	}

	friend class FontResource;
};

const std::string uvHeader = "fft_uv\n";
const std::string bmpHeader = "fft_bmp\n";

FontResource::FontResource()
{
	_data = new FontResourceInternal();
	uvMap = new std::unordered_map<uint32_t, Vector2D>();
}

FontResource::~FontResource()
{
	if (isLoaded)
	{
		DestroyResource();
		isLoaded = false;
	}

	delete uvMap;
}

Vec2D FontResource::UVForGlyph(uint32_t glyph)
{
	auto iter = uvMap->find(glyph);
	if (iter != uvMap->end())
	{
		return iter->second;
	}
	else
	{
		Log(LogLevel_Error, "Trying to access glyph %i but doesn't exists in font !\n", glyph);
		return Vector2D::Zero;
	}
}

void FontResource::CreateBMPForGlyphs(const std::vector<uint32_t>& glyphs)
{
	if (isLoaded == false)
	{
		Log(LogLevel_Error, "Trying to create BMP glyph for not loaded font. \n");
		return;
	}

	if (glyphs.size() == 0)
	{
		Log(LogLevel_Error, "Can not Create 0 Glyphs For Font !\n");
		return;
	}

	bool appendBitMap = false;

	if (_data->bitmapData)
	{
		Log(LogLevel_Verbose, "Already Created font bitmap, creating new and appending \n");
		appendBitMap = true;
	}

	if (glyphs.size() == 1)
	{
		Shape shape;
		if (loadGlyph(shape, _data->font, glyphs[0])) {
			shape.normalize();
			//                      max. angle
			edgeColoringSimple(shape, 3.0);
			//           image width, height
			Bitmap<FloatRGB>* msdf = new Bitmap<FloatRGB>(32, 32);
			//                     range, scale, translation
			generateMSDF(*msdf, shape, 4.0, 1.0, Vector2(4.0, 4.0));

			if (appendBitMap)
			{
				Bitmap<FloatRGB>* old = _data->bitmapData;
				Bitmap<FloatRGB>* nw = new Bitmap<FloatRGB>(old->width() + 32, old->height() + 32);
				Insert(*nw, *old, { 0,0 });
				Insert(*nw, *msdf, { old->width(), old->height() });

				_data->bitmapData = nw;

				delete old;
				delete msdf;
			}
			else
			{
				_data->bitmapData = msdf;
			}

			uvMap->insert({ glyphs[0], {0,0} });
		}
		else
		{
			Log(LogLevel_Warning, "Failed to load Glyph %i for Font\n",glyphs[0]);
		}
	}
	else
	{
		int size_x = static_cast<int>(sqrt(glyphs.size()));
		if (size_x * size_x != glyphs.size())
		{
			size_x++;
		}

		bool resizeData = false;
		if (appendBitMap)
		{
			int old_size = (_data->bitmapData->width() / 32);
			if (old_size < size_x)
			{
				resizeData = true;
			}
			else
			{
				resizeData = false;
				size_x = old_size;
			}
		}

		int x = 0;
		int y = 0;

		std::vector<BMPLoadDataPart<FloatRGB>> msdfs;

		for (uint32_t glyph : glyphs)
		{
			Shape shape;
			if (loadGlyph(shape, _data->font, glyph))
			{
				shape.normalize();
				//                      max. angle
				edgeColoringSimple(shape, 3.0);
				//           image width, height
				Bitmap<FloatRGB>* msdf = new Bitmap<FloatRGB>(32, 32);
				//                     range, scale, translation
				generateMSDF(*msdf, shape, 4.0, 1.0, Vector2(4.0, 4.0));

				msdfs.push_back({ msdf ,{x,y} , glyph});

				x++;
				if (x >= size_x)
				{
					x = 0;
					y++;
				}
			}
			else
			{
				Log(LogLevel_Warning, "Failed to load Glyph %i for Font\n", glyph);
			}
		}

		if (appendBitMap)
		{
			if (resizeData == false)
			{
				Bitmap<FloatRGB>* old = _data->bitmapData;
				Bitmap<FloatRGB>* nw = new Bitmap<FloatRGB>(size_x * 32, size_x * 32);
				Insert(*nw, *old, { 0,0 });

				_data->bitmapData = nw;

				delete old;
			}
		}
		else
		{
			_data->bitmapData = new Bitmap<FloatRGB>(size_x * 32, size_x * 32);
		}

		Vector2I offset;
		Vector2I lastOffset = _data->lastOffset;

		if (appendBitMap)
		{
			for (auto& part : msdfs)
			{
				part.offset.x++;
				part.offset += lastOffset;
				if (part.offset.x >= size_x)
				{
					part.offset.x -= size_x;
					part.offset.y++;
				}
			}
		}

		for (auto part : msdfs)
		{
			offset = part.offset;
			Bitmap<FloatRGB> *bmp = part.bmp;

			Insert(*_data->bitmapData, *bmp, offset * 32);

			Vector2D uvOffset(offset);

			uvOffset /= static_cast<float>(size_x);

			uvMap->insert({ part.glyph,uvOffset });

			delete bmp;
		}

		_data->lastOffset = offset;

	}
	saveBmp(*_data->bitmapData, "test.bmp");
}

int FontResource::LoadFromFile(const std::string& file)
{
	if (file.empty())
	{
		Log(LogLevel_Error, "Trying to load Empty TTF File ! \n");
		return RESOURCE_ERROR_INCORRECT_FILE_TYPE;
	}

	std::string fileType = StringToLower(GetStringFileType(file));

	if (fileType != "ttf" && fileType != "fft")
	{
		Log(LogLevel_Error, "Trying to load Incorrect File Type For Font ! \n Type was %s \n", fileType.c_str());
		return RESOURCE_ERROR_INCORRECT_FILE_TYPE;
	}

	// we are loading a ttf file so we must generate the bmp for it
	if (fileType == "ttf")
	{
		FreetypeHandle* ft = initializeFreetype();
		if (ft) 
		{
			FontHandle *font = loadFont(ft, file.c_str());
			if (font) 
			{
				_data->ft = ft;
				_data->font = font;
			}
			else
			{
				deinitializeFreetype(ft);
				return RESOURCE_ERROR_ERROR_LOADING_FILE;
			}
		}
	}
	// load already paresed fft file
	else if (fileType == "fft")
	{
	}

	isLoaded = true;

	return RESOURCE_ERROR_NO_ERROR;
}

int FontResource::SaveToFile(const std::string & file)
{
	if (isLoaded == false)
	{
		Log(LogLevel_Warning, "Trying to save unloaded font !\n");
		return RESOURCE_ERROR_ERROR_SAVING_FILE;
	}

	std::string filePath = file;
	
	if (GetStringFileType(StringToLower(filePath)) != "fft")
	{
		filePath += ".fft";
	}

	std::string data = uvHeader;

	for (auto iter : *uvMap)
	{
		uint32_t glyph = iter.first;
		Vec2D uv = iter.second;
		
		char bytes[sizeof(glyph) + sizeof(uv)];
		memcpy(bytes, &glyph, sizeof(glyph));
		memcpy(bytes + sizeof(glyph), &uv, sizeof(uv));

		data.append(bytes);
		data += "\n";
	}

	data += bmpHeader;

	return RESOURCE_ERROR_NO_ERROR;

}

void FontResource::DestroyResource()
{
	if (_data)delete _data;
	if(fontTexture)delete fontTexture;
}

const char * FontResource::GetResourceDescription()const
{
	return "FontResource - Wrapper for a loaded font that includes texture for rendering font and map of uv coords to each glyph";
}
