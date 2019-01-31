#include "stdafx.h"
#include "FontAsset.h"

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

class FontAssetInternal
{
private:
	FreetypeHandle* ft;
	FontHandle *font;
	Bitmap<FloatRGB>* msdfData;
	Bitmap<float>* sdfData;
	Bitmap<float>* psdfData;
	Vector2I lastOffset;
	size_t lastSize;

	FontAssetInternal() : msdfData(0), sdfData(0), psdfData(0), ft(0), font(0), lastSize(0)
	{}
	~FontAssetInternal() 
	{
		if (font)destroyFont(font);
		if (ft)deinitializeFreetype(ft);
		if (msdfData)delete msdfData;
		if (sdfData)delete sdfData;
		if (psdfData)delete psdfData;
	}

	friend class FontAsset;
};

const std::string resolutionHeader = "zft_resolution";
const std::string zftHeader = "zft_version_1_0";
const std::string sourceHeader = "zft_source";
const std::string bmpHeader = "zft_png";
const std::string uvHeader = "zft_uv";
const std::string rangeHeader = "zft_range";

const unsigned numHeaders = 5;

Glyph GlyphForShape(const Shape& shape, uint32_t uni, int resolution, double advance, float uvAdvance, Vector2 &translate, Vector2 &scale);

FontAsset::FontAsset(uint32_t resolution, float pxRange, FontSDFType type)
{
	_data = new FontAssetInternal();
	glyphMap = new google::dense_hash_map<uint32_t, Glyph>();
	INIT_MAP_POINTER(glyphMap)
	bmpResolution = resolution;
	sourcePath = new std::string;
	zSourcePath = new std::string;
	this->pxRange = pxRange;
	this->type = type;
}

FontAsset::FontAsset()
{
	_data = new FontAssetInternal();
	glyphMap = new google::dense_hash_map<uint32_t, Glyph>();
	INIT_MAP_POINTER(glyphMap)
	bmpResolution = 0;
	sourcePath = new std::string;
	zSourcePath = new std::string;
	this->pxRange = 0;
}

void FontAsset::GenerateFromSDF(const std::vector<uint32_t>& glyphs)
{
	if (isLoaded == false)
	{
		Log(LogLevel_Error, "Trying to create BMP glyph for not loaded font.");
		return;
	}

	if (glyphs.size() == 0)
	{
		Log(LogLevel_Error, "Can not Create 0 Glyphs For Font !");
		return;
	}

	bool appendBitMap = false;


	int size_x = static_cast<int>(sqrt(glyphs.size()));
	if (size_x * size_x != glyphs.size())
	{
		size_x++;
	}

	bool resizeData = false;
	if (appendBitMap)
	{
		int old_size = (_data->sdfData->width() / bmpResolution);
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

	std::vector<BMPLoadDataPart<float>> msdfs;

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
			Bitmap<float>* msdf = new Bitmap<float>(bmpResolution, bmpResolution);

			Vector2 scale, translate;

			Glyph glyph = GlyphForShape(shape, uni, bmpResolution, advance, 1.0f / (float)size_x, translate, scale);

			//                     range, scale, translation
			//generateMSDF(*msdf, shape, pxRange, scale, translate);
			generateSDF(*msdf, shape, pxRange, scale, translate);

			msdfs.push_back({ msdf ,{ x,y } ,  glyph });

			x++;
			if (x >= size_x)
			{
				x = 0;
				y++;
			}
		}
		else
		{
			Log(LogLevel_Warning, "Failed to load Glyph %i for Font %s", uni, sourcePath->c_str());
		}
	}

	if (appendBitMap)
	{
		if (resizeData)
		{
			Bitmap<float>* old = _data->sdfData;
			Bitmap<float>* nw = new Bitmap<float>(size_x * bmpResolution, size_x * bmpResolution);
			Insert(*nw, *old, { 0,0 });

			_data->sdfData = nw;

			delete old;
		}
	}
	else
	{
		if (_data->sdfData)delete _data->sdfData;
		_data->sdfData = new Bitmap<float>(size_x * bmpResolution, size_x * bmpResolution);
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
		Bitmap<float> *bmp = part.bmp;

		Insert(*_data->sdfData, *bmp, offset * bmpResolution);

		Vector2D uvOffset(offset);

		uvOffset /= static_cast<float>(size_x);

		Glyph glyph = part.glyph;
		glyph.UVOffset = uvOffset;

		glyphMap->insert({ glyph.glyph,  glyph });

		delete bmp;
	}

	_data->lastOffset = offset;
	_data->lastSize = glyphs.size();

	float* pixdata = PixDataForBitmap(*_data->sdfData);

	fontTexture = rEngine->CreateTexture((void*)pixdata, Render_Data_Type_R_8, Render_Data_Format_Float, { _data->sdfData->width(), _data->sdfData->height() });

	free(pixdata);
}

void FontAsset::GenerateFromPSDF(const std::vector<uint32_t>& glyphs)
{
	if (isLoaded == false)
	{
		Log(LogLevel_Error, "Trying to create BMP glyph for not loaded font.");
		return;
	}

	if (glyphs.size() == 0)
	{
		Log(LogLevel_Error, "Can not Create 0 Glyphs For Font !");
		return;
	}

	bool appendBitMap = false;


	int size_x = static_cast<int>(sqrt(glyphs.size()));
	if (size_x * size_x != glyphs.size())
	{
		size_x++;
	}

	bool resizeData = false;
	if (appendBitMap)
	{
		int old_size = (_data->psdfData->width() / bmpResolution);
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

	std::vector<BMPLoadDataPart<float>> msdfs;

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
			Bitmap<float>* msdf = new Bitmap<float>(bmpResolution, bmpResolution);

			Vector2 scale, translate;

			Glyph glyph = GlyphForShape(shape, uni, bmpResolution, advance, 1.0f / (float)size_x, translate, scale);

			//                     range, scale, translation
			//generateMSDF(*msdf, shape, pxRange, scale, translate);
			generatePseudoSDF(*msdf, shape, pxRange, scale, translate);

			msdfs.push_back({ msdf ,{ x,y } ,  glyph });

			x++;
			if (x >= size_x)
			{
				x = 0;
				y++;
			}
		}
		else
		{
			Log(LogLevel_Warning, "Failed to load Glyph %i for Font %s", uni, sourcePath->c_str());
		}
	}

	if (appendBitMap)
	{
		if (resizeData)
		{
			Bitmap<float>* old = _data->psdfData;
			Bitmap<float>* nw = new Bitmap<float>(size_x * bmpResolution, size_x * bmpResolution);
			Insert(*nw, *old, { 0,0 });

			_data->psdfData = nw;

			delete old;
		}
	}
	else
	{
		if (_data->psdfData)delete _data->psdfData;
		_data->psdfData = new Bitmap<float>(size_x * bmpResolution, size_x * bmpResolution);
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
		Bitmap<float> *bmp = part.bmp;

		Insert(*_data->psdfData, *bmp, offset * bmpResolution);

		Vector2D uvOffset(offset);

		uvOffset /= static_cast<float>(size_x);

		Glyph glyph = part.glyph;
		glyph.UVOffset = uvOffset;

		glyphMap->insert({ glyph.glyph,  glyph });

		delete bmp;
	}

	_data->lastOffset = offset;
	_data->lastSize = glyphs.size();

	float* pixdata = PixDataForBitmap(*_data->psdfData);

	fontTexture = rEngine->CreateTexture((void*)pixdata, Render_Data_Type_R_8, Render_Data_Format_Float, { _data->psdfData->width(), _data->psdfData->height() });

	free(pixdata);
}

void FontAsset::GenerateFromMSDF(const std::vector<uint32_t>& glyphs)
{
	if (isLoaded == false)
	{
		Log(LogLevel_Error, "Trying to create BMP glyph for not loaded font.");
		return;
	}

	if (glyphs.size() == 0)
	{
		Log(LogLevel_Error, "Can not Create 0 Glyphs For Font !");
		return;
	}

	bool appendBitMap = false;


	int size_x = static_cast<int>(sqrt(glyphs.size()));
	if (size_x * size_x != glyphs.size())
	{
		size_x++;
	}

	bool resizeData = false;
	if (appendBitMap)
	{
		int old_size = (_data->msdfData->width() / bmpResolution);
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
			//generateMSDF(*msdf, shape, pxRange, scale, translate);
			generateMSDF(*msdf, shape, pxRange, scale, translate);

			msdfs.push_back({ msdf ,{ x,y } ,  glyph });

			x++;
			if (x >= size_x)
			{
				x = 0;
				y++;
			}
		}
		else
		{
			Log(LogLevel_Warning, "Failed to load Glyph %i for Font %s", uni, sourcePath->c_str());
		}
	}

	if (appendBitMap)
	{
		if (resizeData)
		{
			Bitmap<FloatRGB>* old = _data->msdfData;
			Bitmap<FloatRGB>* nw = new Bitmap<FloatRGB>(size_x * bmpResolution, size_x * bmpResolution);
			Insert(*nw, *old, { 0,0 });

			_data->msdfData = nw;

			delete old;
		}
	}
	else
	{
		if (_data->msdfData)delete _data->sdfData;
		_data->msdfData = new Bitmap<FloatRGB>(size_x * bmpResolution, size_x * bmpResolution);
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

		Insert(*_data->msdfData, *bmp, offset * bmpResolution);

		Vector2D uvOffset(offset);

		uvOffset /= static_cast<float>(size_x);


		Glyph glyph = part.glyph;
		glyph.UVOffset = uvOffset;
		glyph.translate /= (float)bmpResolution;
		//glyph.translate = 0;
		glyphMap->insert({ glyph.glyph,  glyph });

		delete bmp;
	}

	_data->lastOffset = offset;
	_data->lastSize = glyphs.size();

	float* pixdata = PixDataForBitmap(*_data->msdfData);

	fontTexture = rEngine->CreateTexture((void*)pixdata, Render_Data_Type_RGB_24, Render_Data_Format_Float, { _data->msdfData->width(), _data->msdfData->height() });

	free(pixdata);
}

void FontAsset::NormalizeGlyphs()
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

FontAsset::~FontAsset()
{
	if (isLoaded)
	{
		if (_data)delete _data;
		isLoaded = false;
	}

	delete zSourcePath;
	delete sourcePath;
	delete glyphMap;
}

const Glyph& FontAsset::GlyphForUnicode(uint32_t uni)
{
	auto iter = glyphMap->find(uni);
	if (iter != glyphMap->end())
	{
		return iter->second;
	}
	else
	{
		Log(LogLevel_Error, "Trying to access glyph %i but doesn't exists in font !", uni);
		return glyphMap->begin()->second;
	}
}

void FontAsset::CreateBMPForGlyphs(const std::vector<uint32_t>& glyphs)
{
	switch (type)
	{
	case Font_SDF_Type_SDF:
		GenerateFromSDF(glyphs);
		break;
	case Font_SDF_Type_PSDF:
		GenerateFromPSDF(glyphs);
		break;
	case Font_SDF_Type_MSDF:
		GenerateFromMSDF(glyphs);
		break;
	}
	
}

void FontAsset::CreateBMPForASCII(const char * ascii)
{
	if (ascii == 0)
	{
		Log(LogLevel_Error, "Font Resource Can not create ascii BMP from null pointer");
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

int FontAsset::MakeFromFile(const std::string & file)
{
	if (file.empty())
	{
		Log(LogLevel_Error, "Trying to load Empty TTF File !");
		return RESOURCE_ERROR_INCORRECT_FILE_TYPE;
	}

	std::string fileType = StringToLower(GetStringFileType(file));

	if (fileType != "ttf")
	{
		Log(LogLevel_Error, "Trying to load Incorrect File Type For Font ! \n Type was %s", fileType.c_str());
		return RESOURCE_ERROR_INCORRECT_FILE_TYPE;
	}

	FreetypeHandle* ft = initializeFreetype();
	if (ft)
	{
		FontHandle *font = loadFont(ft, file.c_str());
		if (font)
		{
			_data->ft = ft;
			_data->font = font;
			*sourcePath = file.c_str();
		}
		else
		{
			deinitializeFreetype(ft);
			return RESOURCE_ERROR_LOADING_FILE;
		}
	}

	return RESOURCE_ERROR_NO_ERROR;
}

int FontAsset::LoadFromFile(const std::string& file)
{
	if (file.empty())
	{
		Log(LogLevel_Error, "Trying to load Empty TTF File !");
		return RESOURCE_ERROR_INCORRECT_FILE_TYPE;
	}

	std::string fileType = StringToLower(GetStringFileType(file));

	if (fileType != "zft")
	{
		Log(LogLevel_Error, "Trying to load Incorrect File Type For Font ! \n Type was %s", fileType.c_str());
		return RESOURCE_ERROR_INCORRECT_FILE_TYPE;
	}


	std::fstream fileS(file, std::ios::in | std::ios::binary);

	if (fileS.good() == false)
	{
		char errorStr[256] = { 0 };
		strerror_s(errorStr, errno);
		Log(LogLevel_Error, "Error opening file %s : %s", file.c_str(), errorStr);
		return RESOURCE_ERROR_LOADING_FILE;
	}

	*zSourcePath = file;

	std::string currentHeader;
	std::getline(fileS, currentHeader, '\n');

	if (currentHeader != zftHeader)
	{
		Log(LogLevel_Error, "File %s Does not Contain Correct Header !!",file.c_str());
		zSourcePath->clear();

		return RESOURCE_ERROR_LOADING_FILE;
	}

	unsigned foundHeaders = 0;

	while (fileS.eof() == false)
	{
		currentHeader.clear();
		std::getline(fileS, currentHeader);

		if (currentHeader == bmpHeader)
		{
			fileS.read((char*)&type, sizeof(type));

			size_t size = 0;
			fileS.read((char*)&size, sizeof(size_t));
			char* cData = new char[size];
			fileS.read(cData, size);

			unsigned char* decoded = 0;
			unsigned int w, h;

			lodepng_decode_memory(&decoded, &w, &h, (unsigned char*)cData, size, LCT_RGB, 8);
			switch (type)
			{
			case Font_SDF_Type_SDF:
				_data->sdfData = FloatBitmapFromMemory(decoded, w, h);
				break;
			case Font_SDF_Type_PSDF:
				_data->psdfData = FloatBitmapFromMemory(decoded, w, h);
				break;
			case Font_SDF_Type_MSDF:
				_data->msdfData = BitmapFromMemory(decoded, w, h);
				break;
			}
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
		else if (currentHeader == rangeHeader)
		{
			fileS.read((char*)&pxRange, sizeof(pxRange));
		}
		else if (currentHeader.empty())
		{
			continue;
		}
		else
		{
			Log(LogLevel_Error, "Invalid Line Loading %s", file.c_str());

			zSourcePath->clear();
			sourcePath->clear();
			glyphMap->clear();

			return RESOURCE_ERROR_LOADING_FILE;
		}

		foundHeaders++;
	}

	if (foundHeaders != numHeaders)
	{
		Log(LogLevel_Error, "Didn't find all sections of file %s\n", file.c_str());

		zSourcePath->clear();
		sourcePath->clear();
		glyphMap->clear();
		if (_data->sdfData)delete _data->sdfData;
		if (_data->psdfData)delete _data->psdfData;
		if (_data->msdfData)delete _data->msdfData;

		return RESOURCE_ERROR_LOADING_FILE;
	}

	isLoaded = true;

	return RESOURCE_ERROR_NO_ERROR;
}

int FontAsset::SaveToFile(const std::string & file)
{
	// TODO: Save and load pxRange

	if (isLoaded == false)
	{
		Log(LogLevel_Error, "Trying to save unloaded font !\n");
		return RESOURCE_ERROR_NOT_LOADED;
	}

	std::string filePath = file;
	
	if (GetStringFileType(StringToLower(filePath)) != "zft")
	{
		filePath += ".zft";
	}

	std::fstream fileS(*zSourcePath, std::ios::out | std::ios::trunc | std::ios::binary);

	if (fileS.good() == false)
	{
		char errorStr[256] = { 0 };
		strerror_s(errorStr, errno);
		Log(LogLevel_Error, "Error opening file %s : %s", filePath.c_str(), errorStr);
		return RESOURCE_ERROR_SAVING_FILE;
	}

	fileS << zftHeader + "\n";

	fileS << uvHeader + "\n";
	{
		size_t size = glyphMap->size();
		fileS.write((const char*)&size, sizeof(size_t));
		// for efficeicny
		char bytes[sizeof(Glyph)];

		for (auto iter : *glyphMap)
		{
			Glyph glyph = iter.second;

			memcpy(bytes, &glyph, sizeof(glyph));
			fileS.write((char*)&glyph,sizeof(glyph));
		}
	}

	fileS << resolutionHeader + "\n";
	{
		fileS.write((char*)&bmpResolution,sizeof(bmpResolution));
		fileS << "\n";
	}

	fileS << rangeHeader + "\n";
	{
		fileS.write((char*)&pxRange, sizeof(pxRange));
		fileS << "\n";
	}

	fileS << bmpHeader + "\n";
	{
		size_t outSize = 0;
		unsigned char* bdata = 0;

		fileS.write((char*)&type, sizeof(type));

		switch (type)
		{
		case Font_SDF_Type_SDF:
			if (_data->sdfData == 0)
			{
				Log(LogLevel_Error, "Trying to save font without butmap data !!. \n");
				return RESOURCE_ERROR_SAVING_FILE;
			}

			encodePng(&bdata, &outSize, *_data->sdfData);
			break;
		case Font_SDF_Type_PSDF:
			if (_data->psdfData == 0)
			{
				Log(LogLevel_Error, "Trying to save font without butmap data !!. \n");
				return RESOURCE_ERROR_SAVING_FILE;
			}

			encodePng(&bdata, &outSize, *_data->psdfData);
			break;
		case Font_SDF_Type_MSDF:
			if (_data->msdfData == 0)
			{
				Log(LogLevel_Error, "Trying to save font without butmap data !!. \n");
				return RESOURCE_ERROR_SAVING_FILE;
			}

			encodePng(&bdata, &outSize, *_data->msdfData);
			break;
		}

		fileS.write((const char*)&outSize, sizeof(outSize));
		fileS.write((const char*)bdata, outSize);

		fileS << "\n";

		delete bdata;
	}

	fileS << sourceHeader + "\n";
	{
		fileS << *sourcePath + "\n";
	}

	*zSourcePath = filePath;

	fileS.close();

	return RESOURCE_ERROR_NO_ERROR;

}

int FontAsset::Save()
{
	if (isLoaded == false)
	{
		return RESOURCE_ERROR_NOT_LOADED;
	}

	if (zSourcePath->empty())
	{
		Log(LogLevel_Error, "Could not re-save file without original");
		return RESOURCE_ERROR_SAVING_FILE;
	}

	return SaveToFile(*zSourcePath);
}

const char * FontAsset::GetResourceDescription()const
{
	return "FontAsset - Wrapper for a loaded font that includes texture for rendering font and map of uv coords to each glyph";
}

Glyph GlyphForShape(const Shape& shape, uint32_t uni, int resolution, double advance, float uvAdvance, Vector2 &translate, Vector2 &scale)
{
	Glyph glyph;

	glyph.advance = ((float)advance / (float)resolution) * 0.8f;
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

	scale = frame / dims;
	scale *= 0.70f;

	Vector2D tr((float)dims.x * 0.15f, (float)dims.y *0.15f);

	translate = Vector2D((float)(-l + tr.x), (float)(-b + tr.y));

	glyph.translate = Vector2D((float)tr.x, (float)tr.y);
	glyph.bearing = { (float)-l / (float)resolution, (float)-b / (float)resolution };
	glyph.size = { (float)dims.x / (float)resolution,(float)dims.y / (float)resolution };
	glyph.scale = { (float)scale.x, (float)scale.y };
	glyph.invScale = 1.0f / glyph.scale;
	glyph.scaleFactor = 0.70f;
	return glyph;
}