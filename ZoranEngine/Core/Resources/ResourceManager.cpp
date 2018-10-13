#include "stdafx.h"
#include "ResourceManager.h"

#include "FontResource.h"
ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
}

FontResource * ResourceManager::FontForTTF(const char * ttf, uint32_t resolution)
{
	FontResource* font = new FontResource(resolution);
	font->LoadFromFile(ttf);
	return font;
}

FontResource * ResourceManager::FontForZFT(const char * zft)
{
	FontResource* font = new FontResource(0);
	font->LoadFromFile(zft);
	return font; return nullptr;
}
