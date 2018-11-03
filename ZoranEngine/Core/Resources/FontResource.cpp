#include "stdafx.h"
#include "FontResource.h"

#include <fstream>
#include <stdio.h>
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

const std::string resolutionHeader = "zft_resolution";
const std::string zftHeader = "zft_version_1_0";
const std::string sourceHeader = "zft_source";
const std::string bmpHeader = "zft_png";
const std::string uvHeader = "zft_uv";

const unsigned numHeaders = 4;

Glyph GlyphForShape(const Shape& shape, uint32_t uni, int resolution, double advance, float uvAdvance, Vector2 &translate, Vector2 &scale);

FontResource::FontResource(uint32_t resolution)
{
	_data = new FontResourceInternal();
	glyphMap = new std::unordered_map<uint32_t, Glyph>();
	bmpResolution = resolution;
	sourcePath = new std::string;
	zSourcePath = new std::string;
}

void FontResource::NormalizeGlyphs()
{
	Vector2D maxSize;

	for (auto& itr : *glyphMap)
	{
		Glyph &g = itr.second;

		if (g.size.w > maxSize.w)maxSize.w = g.size.w;
		if (g.size.h > maxSize.h)maxSize.h = g.size.h;
	}

	float theMax = maxSize.getMaxValue();

	for (auto& itr : *glyphMap)
	{
		Glyph &g = itr.second;

		g.size /= theMax;
		g.advance /= theMax;
		g.bearing /= theMax;
	}
}

FontResource::~FontResource()
{
	if (isLoaded)
	{
		DestroyResource();
		isLoaded = false;
	}

	delete zSourcePath;
	delete sourcePath;
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
			
			Vector2 scale, translate;

			Glyph glyph = GlyphForShape(shape, uni, bmpResolution, advance, 1.0f / (float)size_x, translate, scale);

			//                     range, scale, translation
			generateMSDF(*msdf, shape, 4.0, scale, translate);

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
		*sourcePath = file.c_str();
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
		*zSourcePath = file.c_str();

		std::fstream fileS(file, std::ios::in | std::ios::binary);

		std::string currentHeader;
		std::getline(fileS, currentHeader, '\n');

		if (currentHeader != zftHeader)
		{
			Log(LogLevel_Error, "File %s Does not Contain Correct Header !!\n",file);
			zSourcePath->clear();

			return RESOURCE_ERROR_ERROR_LOADING_FILE;
		}

		unsigned foundHeaders = 0;

		while (fileS.eof() == false)
		{
			currentHeader.clear();
			std::getline(fileS, currentHeader);

			if (currentHeader == bmpHeader)
			{
				size_t size = 0;
				fileS.read((char*)&size, sizeof(size_t));
				char* cData = new char[size];
				fileS.read(cData, size);

				unsigned char* decoded = 0;
				unsigned int w, h;

				lodepng_decode_memory(&decoded, &w, &h, (unsigned char*)cData, size, LCT_RGB, 8);

				fontTexture = rEngine->CreateTexture((void*)decoded, Render_Data_Type_RGB_24, Render_Data_Format_Unsigned_Byte, { (int)w,(int)h });

				delete cData;
				delete decoded;
			}
			else if (currentHeader == uvHeader)
			{
				size_t size = 0;
				fileS.read((char*)&size, sizeof(size_t));

				for (size_t i=0; i < size; i++)
				{
					Glyph glyph;
					fileS.read((char*)&glyph, sizeof(Glyph));
					glyphMap->insert({ glyph.glyph,glyph });
				}
			}
			else if (currentHeader == sourceHeader)
			{
				std::getline(fileS, *sourcePath);
			}
			else if (currentHeader == resolutionHeader)
			{
				fileS.read((char*)&bmpResolution, sizeof(bmpResolution));
			}
			else if (currentHeader.empty())
			{
				continue;
			}
			else
			{
				Log(LogLevel_Error, "Invalid Line Loading %s\n", file);

				zSourcePath->clear();
				sourcePath->clear();
				glyphMap->clear();

				return RESOURCE_ERROR_ERROR_LOADING_FILE;
			}

			foundHeaders++;
		}

		if (foundHeaders != numHeaders)
		{
			Log(LogLevel_Error, "Didn't find all sections of file %s\n", file);

			zSourcePath->clear();
			sourcePath->clear();
			glyphMap->clear();

			return RESOURCE_ERROR_ERROR_LOADING_FILE;
		}
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

	std::string data = zftHeader + "\n";

	data += uvHeader + "\n";
	{
		size_t size = glyphMap->size();
		data.append((const char*)&size, sizeof(size_t));
		// for efficeicny
		char bytes[sizeof(Glyph)];

		for (auto iter : *glyphMap)
		{
			Glyph glyph = iter.second;

			memcpy(bytes, &glyph, sizeof(glyph));
			data.append((char*)&glyph,sizeof(glyph));
		}
	}

	data += resolutionHeader + "\n";
	{
		data.append((char*)&bmpResolution,sizeof(bmpResolution));
		data += "\n";
	}

	data += bmpHeader + "\n";
	{
		size_t outSize = 0;
		unsigned char* bdata = 0;

		encodePng(&bdata, &outSize, *_data->bitmapData);

		data.append((const char*)&outSize,sizeof(outSize));
		data.append(bdata, bdata + outSize);
		data += "\n";

		delete bdata;
	}

	data += sourceHeader + "\n";
	{
		data += *sourcePath + "\n";
	}

	*zSourcePath = filePath;

	std::fstream files(*zSourcePath,std::ios::out | std::ios::trunc | std::ios::binary);

	files.write(data.c_str(), data.size());

	files.close();

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

Glyph GlyphForShape(const Shape& shape, uint32_t uni, int resolution, double advance, float uvAdvance, Vector2 &translate, Vector2 &scale)
{
	Glyph glyph;

	glyph.advance = advance / (double)resolution;
	glyph.glyph = uni;
	glyph.uvAdvance = uvAdvance;

	double l , b, r, t;

	l = b = r = t = 0;

	shape.bounds(l, b, r, t);

	Vector2 frame(resolution, resolution);

	if (l >= r || b >= t)
		l = 0, b = 0, r = 1, t = 1;
	//if (frame.x <= 0 || frame.y <= 0)
		//assert("Cannot fit the specified pixel range.");
	Vector2 dims(r - l, t - b);

	translate = Vector2D((float)-l, (float)-b);

	//scale = frame.y / dims.y ;
	scale = frame / dims;

	glyph.bearing = { (float)translate.x / (float)resolution, (float)translate.y / (float)resolution };
	glyph.size = { (float)dims.x / (float)resolution,(float)dims.y / (float)resolution };
	glyph.invScale = { 1.0f / (float)scale.x, 1.0f / (float)scale.y };;
	return glyph;
}