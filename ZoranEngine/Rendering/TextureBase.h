#pragma once
#include "Core/PlatformTypes.h"
#include "Rendering/RenderTypes.h"
#include <Math/Vector2.h>
/*
* This represents a texture stored in gpu memory. This calss is not supposed to be created manually but made through a RenderEngine
*/

class RenderEngineBase;
class ZoranEngine_EXPORT TextureBase
{
protected:

	RenderDataType type; // represents the type of the data stored
	RenderDataFormat format; // represents the format of the data stored
	bool containsData; // simply wether or not this texture has valid texture data

	unsigned width, height;

public:
	const char* path; // used for texture manager keying

protected:
	unsigned LoadFromPNG(const char* path,unsigned &x, unsigned &y, unsigned char ** data);
	virtual ~TextureBase();

public:
	TextureBase(unsigned width,unsigned height,RenderDataType type = Render_Data_Type_BGRA_32, RenderDataFormat format = Render_Data_Format_Unsigned_Byte);

	RenderDataType GetRenderDataType()const { return type; }
	RenderDataFormat GetRenderDataFormat()const { return format; }
	
	bool GetContainsData()const { return containsData; }

	/* 
	* in bot LoadFromPath and Memory the type and format represent the passed datas type and memory so in the internal type is GL_BGRA 
	* and the passed data is RGBA then the data will be internally converted to BGRA 
	*/

	virtual void LoadFromPath(const char* texture_path, RenderDataType type = Render_Data_Type_BGRA_32, RenderDataFormat format = Render_Data_Format_Unsigned_Byte);
	virtual void LoadFromMemory(unsigned x, unsigned y, void* data, RenderDataType type = Render_Data_Type_BGRA_32, RenderDataFormat format = Render_Data_Format_Unsigned_Byte) = 0;

	// these must be pure virtual because the data may have to be changed via the render engine to the new format / type

	virtual void SetRenderDataType(RenderDataType newType) = 0;
	virtual void SetRenderDataFormat(RenderDataFormat newFormat) = 0;
	/* data argument is available in case d3d needs something to be passed to use OpenGL Textures just use this argument as an int*/
	virtual void UseTexture(unsigned long data)const = 0;
	virtual void StopUsingTexture(unsigned long  data)const = 0;
	/* returns the render engine specific id */
	virtual unsigned GetTextureID()const = 0;

	inline Vector2I GetSize()const { return Vector2I(width, height); }

	friend class TextureManager;
};

