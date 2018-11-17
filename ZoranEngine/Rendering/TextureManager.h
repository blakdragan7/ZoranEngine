#pragma once
#include <map>
#include "RenderTypes.h"

class TextureBase;
class ZoranEngine_EXPORT TextureManager
{
private:
	std::map<const char*, TextureBase*>* textureMap;

	static TextureManager* instance;

public:
	TextureManager();
	~TextureManager();

	void DestroyTexture(TextureBase* texture);
	TextureBase* TextureForFilePath(const char* texture_path, RenderDataType type = Render_Data_Type_BGRA_32, RenderDataFormat format = Render_Data_Format_Unsigned_Byte);

	static TextureManager* GetInstance();
};

#define tManager TextureManager::GetInstance();