#pragma once
#include <map>
#include "RenderTypes.h"

class TextureBase;
class TextureManager
{
private:
	std::map<const char*, TextureBase*> textureMap;

	static TextureManager* instance;

public:
	TextureManager();
	~TextureManager();

	void DestroyTexture(TextureBase* texture);
	TextureBase* TextureForFilePath(const char* texture_path, RenderDataType type = TYPE_BGRA_32, RenderDataFormat format = FORMAT_UNSIGNED_BYTE);

	static TextureManager* GetInstance();
};

#define tManager TextureManager::GetInstance();