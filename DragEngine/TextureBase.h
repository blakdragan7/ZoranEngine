#pragma once
#include "PlatformTypes.h"
#include "RenderTypes.h"

/*
* This represents a texture stored in gpu memory. This calss is not supposed to be created manually but made through a RenderEngine
*/

class RenderEngineBase;
class DRAGENGINE_EXPORT TextureBase
{
protected:

	RenderEngineBase * engine;
	RenderDataType type; // represents the type of the data stored
	RenderDataFormat format; // represents the format of the data stored
	bool containsData; // simply wether or not this texture has valid texture data

	unsigned width, height;

protected:
	unsigned LoadFromPNG(const char* path,unsigned &x, unsigned &y, unsigned char ** data);

public:
	TextureBase(RenderEngineBase* engine, unsigned width,unsigned height,RenderDataType type = TYPE_BGRA_32, RenderDataFormat format = FORMAT_UNSIGNED_BYTE);
	virtual ~TextureBase();

	RenderDataType GetRenderDataType()const { return type; }
	RenderDataFormat GetRenderDataFormat()const { return format; }
	
	bool GetContainsData()const { return containsData; }

	/* 
	* in bot LoadFromPath and Memory the type and format represent the passed datas type and memory so in the internal type is GL_BGRA 
	* and the passed data is RGBA then the data will be internally converted to BGRA 
	*/

	virtual void LoadFromPath(const char* texture_path, RenderDataType type = TYPE_BGRA_32, RenderDataFormat format = FORMAT_UNSIGNED_BYTE) = 0;
	virtual void LoadFromMemory(unsigned x, unsigned y, void* data, RenderDataType type = TYPE_BGRA_32, RenderDataFormat format = FORMAT_UNSIGNED_BYTE) = 0;

	// these must be pure virtual because the data may have to be changed via the render engine to the new format / type

	virtual void SetRenderDataType(RenderDataType newType) = 0;
	virtual void SetRenderDataFormat(RenderDataFormat newFormat) = 0;

	friend class RenderEngineBase;
};

