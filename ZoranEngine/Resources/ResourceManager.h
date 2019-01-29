#pragma once
/*
*	Resource Manager.
*
*	Gives Access to all resources stored on the disk like fonts, images etc..
*	Abstracts loading files to make a cross platform interface.
*/
#include <map>

#include <Rendering/RenderTypes.h>
#include <Resources/ResourceTypes.h>

class ZoranEngine_EXPORT ResourceManager
{
private:
	std::map<std::string, FontResource>*  fontMap;
	std::map<std::string, ImageResource>* imageMap;
	std::map<std::string, SoundResource>* soundMap;
	std::map<std::string, ModelResource>* modelMap;

	static ResourceManager* instance;
	
	ResourceManager();

public:
	~ResourceManager();

	FontResource MakeFontForPath(const std::string& ttfSource, const std::string& resourcePath,uint32_t resolution, float pxRange, unsigned sdfType);
	FontResource FontForPath(const std::string& path);

	ImageResource MakeImageForPath(const std::string& source, const std::string& resourcePath, RenderDataType type = Render_Data_Type_RGBA_32, RenderDataFormat format = Render_Data_Format_Unsigned_Byte);
	ImageResource ImageForPath(const std::string& path);
				 
	SoundResource MakeSoundForPath(const std::string& source, const std::string& resourcePath, AudioListener* listener = 0);
	SoundResource SoundForPath(const std::string& path, AudioListener* listener = 0);
				 
	ModelResource MakeModelForPath(const std::string& source, const std::string& resourcePath);
	ModelResource ModelForPath(const std::string& path);

	void CleanupResources();

	static ResourceManager* GetResourceManager();
};

#define RM ResourceManager::GetResourceManager()
