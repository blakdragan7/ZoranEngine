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
