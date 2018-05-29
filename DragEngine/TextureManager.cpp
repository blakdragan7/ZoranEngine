#include "stdafx.h"
#include "TextureManager.h"
#include "DragEngine.h"
#include "RenderEngineBase.h"

TextureManager* TextureManager::instance = 0;

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
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
