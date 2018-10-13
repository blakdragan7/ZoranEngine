#pragma once
/*
*	Resource Manager.
*
*	Gives Access to all resources stored on the disk like font's shaders etc..
*	Abstracts loading files to make a cross platform interface.
*/

class ResourceBase;
class FontResource;
class ZoranEngine_EXPORT ResourceManager
{
private:

public:
	ResourceManager();
	virtual ~ResourceManager();

	FontResource* FontForTTF(const char* ttf, uint32_t resolution = 32);
	FontResource* FontForZFT(const char* zft);
};

