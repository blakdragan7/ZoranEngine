#include "stdafx.h"
#include "FontResource.h"

#include <fstream>

#include <ThirdParty/msdfgen/msdfgen.h>
#include <ThirdParty/msdfgen/msdfgen-ext.h>

#include <ThirdParty/loadpng/lodepng.h>

#include <Rendering/TextureBase.h>
#include <Utils/StringAddons.hpp>
#include <Utils/BitmapAddons.hpp>

#include <Rendering/RenderEngineBase.h>

#include "ResourceErrors.h"

using namespace msdfgen;
template<typename t>
struct BMPLoadDataPart
{
	Bitmap<t> *bmp;
	Vector2I offset;
	Glyph glyph;
};

class FontResourceInternal
{
private:
	FreetypeHandle* ft;
	FontHandle *font;
	Bitmap<FloatRGB>* bitmapData;
	Vector2I lastOffset;
	size_t lastSize;

	FontResourceInternal() : bitmapData(0), ft(0), font(0), lastSize(0)
	{}
	~FontResourceInternal() 
	{
		if (font)destroyFont(font);
		if (ft)deinitializeFreetype(ft);
		if (bitmapData)delete bitmapData; 
	}

	friend class FontResource;
};

const std::string uvHeader = "zft_uv\n";
const std::string bmpHeader = "zft_png\n";
const std::string sourceHeader = "zft_source\n";

Glyph GlyphForShape(const Shape& shape, uint32_t uni, int resolution, double advance, float uvAdvance, Vector2 &scale);

FontResource::FontResource(uint32_t resolution)
{
	_data = new FontResourceInternal();
	glyphMap = new std::unordered_map<uint32_t, Glyph>();
	bmpResolution = resolution;
}

FontResource::~FontResource()
{
	if (isLoaded)
	{
		DestroyResource();
		isLoaded = false;
	}

	delete glyphMap;
}

const Glyph& FontResource::GlyphForUnicode(uint32_t uni)
{
	auto iter = glyphMap->find(uni);
	if (iter != glyphMap->end())
	{
		return iter->second;
	}
	else
	{
		Log(LogLevel_Error, "Trying to access glyph %i but doesn't exists in font !\n", uni);
		return glyphMap->begin()->second;
	}
}

void FontResource::CreateBMPForGlyphs(const std::vector<uint32_t>& glyphs)
{
	// TODO make sure to properly check for old_size + new size 

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

	
	int size_x = static_cast<int>(sqrt(glyphs.size()));
	if (size_x * size_x != glyphs.size())
	{
		size_x++;
	}

	bool resizeData = false;
	if (appendBitMap)
	{
		int old_size = (_data->bitmapData->width() / bmpResolution);
		if (old_size < size_x)
		{
			resizeData = true;
		}
		else
		{
			int totalSize = (int)sqrt(_data->lastSize + glyphs.size());
			if (totalSize * totalSize >= _data->lastSize + glyphs.size())
			{
				resizeData = false;
				size_x = old_size;
			}
			else
			{
				totalSize++;
				size_x = totalSize;
				resizeData = true;
			}
		}
	}

	int x = 0;
	int y = 0;

	std::vector<BMPLoadDataPart<FloatRGB>> msdfs;

	for (uint32_t uni : glyphs)
	{
		Shape shape;
		double advance = 0;
		if (loadGlyph(shape, _data->font, uni, &advance))
		{
			shape.normalize();
			//                      max. angle
			edgeColoringSimple(shape, 3.0);
			//           image width, height
			Bitmap<FloatRGB>* msdf = new Bitmap<FloatRGB>(bmpResolution, bmpResolution);
			
			Vector2 scale;

			Glyph glyph = GlyphForShape(shape, uni, bmpResolution, advance, 1.0f / (float)size_x, scale);

			//                     range, scale, translation
			generateMSDF(*msdf, shape, 4.0, scale, glyph.bearing);

			msdfs.push_back({ msdf ,{x,y} ,  glyph });

			x++;
			if (x >= size_x)
			{
				x = 0;
				y++;
			}
		}
		else
		{
			Log(LogLevel_Warning, "Failed to load Glyph %i for Font %s\n", uni, sourcePath);
		}
	}

	if (appendBitMap)
	{
		if (resizeData)
		{
			Bitmap<FloatRGB>* old = _data->bitmapData;
			Bitmap<FloatRGB>* nw = new Bitmap<FloatRGB>(size_x * bmpResolution, size_x * bmpResolution);
			Insert(*nw, *old, { 0,0 });

			_data->bitmapData = nw;

			delete old;
		}
	}
	else
	{
		_data->bitmapData = new Bitmap<FloatRGB>(size_x * bmpResolution, size_x * bmpResolution);
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

		Insert(*_data->bitmapData, *bmp, offset * bmpResolution);

		Vector2D uvOffset(offset);

		uvOffset /= static_cast<float>(size_x);

		Glyph glyph = part.glyph;
		glyph.UVOffset = uvOffset;

		glyphMap->insert({ glyph.glyph,  glyph });

		delete bmp;
	}

	_data->lastOffset = offset;
	_data->lastSize = glyphs.size();
	
	if (fontTexture)delete fontTexture;

	float* pixdata = PixDataForBitmap(*_data->bitmapData);

	fontTexture = rEngine->CreateTexture((void*)pixdata, Render_Data_Type_RGB_24, Render_Data_Format_Float, {_data->bitmapData->width(), _data->bitmapData->height()});

	free(pixdata);
}

void FontResource::CreateBMPForASCII(const char * ascii)
{
	if (ascii == 0)
	{
		Log(LogLevel_Error, "Font Resource Can not create ascii BMP from null pointer \n");
		return;
	}

	std::vector<uint32_t> glyphs;

	size_t len = strlen(ascii);
	for (size_t i = 0; i < len; i++)
	{
		glyphs.push_back(ascii[i]);
	}

	CreateBMPForGlyphs(glyphs);
}

int FontResource::LoadFromFile(const std::string& file)
{
	if (file.empty())
	{
		Log(LogLevel_Error, "Trying to load Empty TTF File ! \n");
		return RESOURCE_ERROR_INCORRECT_FILE_TYPE;
	}

	std::string fileType = StringToLower(GetStringFileType(file));

	if (fileType != "ttf" && fileType != "zft")
	{
		Log(LogLevel_Error, "Trying to load Incorrect File Type For Font ! \n Type was %s \n", fileType.c_str());
		return RESOURCE_ERROR_INCORRECT_FILE_TYPE;
	}

	// we are loading a ttf file so we must generate the bmp for it
	if (fileType == "ttf")
	{
		sourcePath = file.c_str();
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
	else if (fileType == "zft")
	{
		zSourcePath = file.c_str();
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
	
	if (GetStringFileType(StringToLower(filePath)) != "zft")
	{
		filePath += ".zft";
	}

	std::string data = uvHeader;

	for (auto iter : *glyphMap)
	{
		uint32_t glyph = iter.first;
		Vec2D uv = iter.second.UVOffset;
		
		char bytes[sizeof(glyph) + sizeof(uv)];
		memcpy(bytes, &glyph, sizeof(glyph));
		memcpy(bytes + sizeof(glyph), &uv, sizeof(uv));

		data.append(bytes);
		data += "\n";
	}


	data += bmpHeader;
	
	{
		size_t outSize = 0;
		unsigned char* bdata = 0;

		encodePng(&bdata, &outSize, *_data->bitmapData);

		data.append(bdata, bdata + outSize);
		data += "\n";
	}

	data += sourceHeader;
	data += sourcePath + std::string("\n");

	zSourcePath = file.c_str();

	std::fstream fileh(zSourcePath,std::ios::out);

	fileh.write(data.c_str(), data.size());

	fileh.close();

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

Glyph GlyphForShape(const Shape& shape, uint32_t uni, int resolution, double advance, float uvAdvance, Vector2 &scale)
{
	Glyph glyph;

	glyph.advance = advance;
	glyph.glyph = uni;
	glyph.uvAdvance = uvAdvance;

	double l , b, r, t;

	l = b = r = t = 0;

	shape.bounds(l, b, r, t);

	Vector2 frame(resolution, resolution);

	Vector2D translate;

	if (l >= r || b >= t)
		l = 0, b = 0, r = 1, t = 1;
	//if (frame.x <= 0 || frame.y <= 0)
		//assert("Cannot fit the specified pixel range.");
	Vector2 dims(r - l, t - b);

	if (dims.x*frame.y < dims.y*frame.x) {
		translate = Vector2D(.5*(frame.x / frame.y*dims.y - dims.x) - l, -b);
		scale = frame.y / dims.y ;
	}
	else {
		translate = Vector2D(-l, .5*(frame.y / frame.x*dims.x - dims.y) - b);
		scale = frame.x / dims.x;
	}

	glyph.bearing = translate;
	glyph.size = { (float)dims.x,(float)dims.y };
	return glyph;
}