#include "stdafx.h"
#include "TextureManager.h"
#include "Core/ZoranEngine.h"
#include "RenderEngineBase.h"

#include <Rendering/TextureBase.h>

TextureManager* TextureManager::instance = 0;

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	for (auto& iter : textureMap)
	{
		delete iter.second;
	}

	textureMap.clear();
}

void TextureManager::DestroyTexture(TextureBase * texture)
{
	auto& iter = textureMap.find(texture->path);

	if (iter != textureMap.end())
	{
		textureMap.erase(iter);
		delete iter->second;
	}
}

TextureBase * TextureManager::TextureForFilePath(const char* texture_path, RenderDataType type, RenderDataFormat format)
{
	auto iter = textureMap.find(texture_path);
	if (iter != textureMap.end())
	{
		return iter->second;
	}
	else
	{
		TextureBase* texture = rEngine->CreateTexture(texture_path,type,format);
		texture->path = texture_path;
		std::pair<const char*, TextureBase*> entry(texture_path, texture);
		textureMap.insert(entry);

		return texture;
	}
}

TextureManager * TextureManager::GetInstance()
{
	if (!instance)instance = new TextureManager();
	return instance;
}
