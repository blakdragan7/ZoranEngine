#include "stdafx.h"
#include "ResourceManager.h"

#include "FontResource.h"
ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
}

FontResource * ResourceManager::FontForTTF(const char * ttf)
{
	FontResource* font = new FontResource();
	font->LoadFromFile(ttf);
	return font;
}
