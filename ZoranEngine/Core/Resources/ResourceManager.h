#pragma once
/*
*	Resource Manager.
*
*	Gives Access to all resources stored on the disk like font's shaders etc..
*	Abstracts loading files to make a cross platform interface.
*/
#include <map>

class ResourceBase;
class FontResource;
class ZoranEngine_EXPORT ResourceManager
{
private:
	std::map<std::string, ResourceBase*>* resourceMap;
	static ResourceManager* instance;
	
	ResourceManager();

public:
	virtual ~ResourceManager();

	FontResource* FontForTTF(const char* ttf, uint32_t resolution, float pxRange, unsigned sdfType);
	FontResource* FontForZFT(const char* zft);

	static ResourceManager* man();
};

#define RM ResourceManager::man()
