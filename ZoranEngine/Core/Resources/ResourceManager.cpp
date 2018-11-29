#include "stdafx.h"
#include "ResourceManager.h"

#include "FontResource.h"

ResourceManager* ResourceManager::instance = 0;

ResourceManager::ResourceManager()
{
	resourceMap = new std::map<std::string, ResourceBase*>;
}

ResourceManager::~ResourceManager()
{
	delete resourceMap;
}

FontResource * ResourceManager::FontForTTF(const char * ttf, uint32_t resolution, float pxRange, unsigned sdfType)
{
	auto itr = resourceMap->find(ttf);
	if (itr != resourceMap->end())
	{					 
		return (FontResource*)itr->second;
	}

	FontResource* font = new FontResource(resolution, pxRange, (FontSDFType)sdfType);
	font->LoadFromFile(ttf);

	resourceMap->insert({ ttf,font });

	return font;
}

FontResource * ResourceManager::FontForZFT(const char * zft)
{
	auto itr = resourceMap->find(zft);
	if (itr != resourceMap->end())
	{
		return (FontResource*)itr->second;
	}

	FontResource* font = new FontResource;
	font->LoadFromFile(zft);
	
	resourceMap->insert({ zft,font });

	return font;
}

ResourceManager * ResourceManager::man()
{
	if (instance == 0)instance = new ResourceManager;
	return instance;
}
