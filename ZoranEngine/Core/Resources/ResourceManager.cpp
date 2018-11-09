#include "stdafx.h"
#include "ResourceManager.h"

#include "FontResource.h"
ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
}

FontResource * ResourceManager::FontForTTF(const char * ttf, uint32_t resolution, float pxRange, unsigned sdfType)
{
	FontResource* font = new FontResource(resolution, pxRange, (FontSDFType)sdfType);
	font->LoadFromFile(ttf);
	return font;
}

FontResource * ResourceManager::FontForZFT(const char * zft)
{
	FontResource* font = new FontResource;
	font->LoadFromFile(zft);
	return font; return nullptr;
}
