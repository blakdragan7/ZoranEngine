#include "stdafx.h"
#include "ResourceManager.h"

#include "ResourceErrors.h"

ResourceManager* ResourceManager::instance = 0;

ResourceManager::ResourceManager()
{
	fontMap  = new std::map<std::string, FontResource>;
	imageMap = new std::map<std::string, ImageResource>;
	soundMap = new std::map<std::string, SoundResource>;
	modelMap = new std::map<std::string, ModelResource>;
}

ResourceManager::~ResourceManager()
{
	delete fontMap;
	delete imageMap;
	delete modelMap;
	delete soundMap;
}

FontResource ResourceManager::MakeFontForPath(const std::string& ttf, const std::string& resourcePath, uint32_t resolution, float pxRange, unsigned sdfType)
{
	FontResource font = FontResource(resolution, pxRange, (FontSDFType)sdfType);
	int error = font->MakeFromFile(ttf);
	if (error != RESOURCE_ERROR_NO_ERROR)
	{
		Log(LogLevel_Error, "Error Loading Font from %s with error %s", ttf.c_str(), StringForResourceError(error));
		return FontResource::Invalid;
	}
	error = font->SaveToFile(resourcePath);
	if (error != RESOURCE_ERROR_NO_ERROR)
	{
		Log(LogLevel_Error, "Error Saving Font to %s with error %s", resourcePath.c_str(), StringForResourceError(error));
		return FontResource::Invalid;
	}

	fontMap->insert({ resourcePath,font });

	return font;
}

FontResource ResourceManager::FontForPath(const std::string& resourcePath)
{
	auto itr = fontMap->find(resourcePath);
	if (itr != fontMap->end())
	{
		return itr->second;
	}

	FontResource font;
	font.DefaultConstruct();

	int err = font->LoadFromFile(resourcePath);
	if (err != RESOURCE_ERROR_NO_ERROR)
	{
		Log(LogLevel_Error, "Could not load font %s iwth error %s", resourcePath.c_str(), StringForResourceError(err));
		return FontResource::Invalid;
	}

	fontMap->insert({ resourcePath,font });

	return font;
}

ImageResource ResourceManager::MakeImageForPath(const std::string & source, const std::string & resourcePath, RenderDataType type, RenderDataFormat format)
{
	ImageResource image;
	image.DefaultConstruct();

	int error = image->MakeFromFile(source, type, format);
	if (error != RESOURCE_ERROR_NO_ERROR)
	{
		Log(LogLevel_Error, "Error Loading image from %s with error %s", source.c_str(), StringForResourceError(error));
		return ImageResource::Invalid;
	}
	error = image->SaveToFile(resourcePath);
	if (error != RESOURCE_ERROR_NO_ERROR)
	{
		Log(LogLevel_Error, "Error Saving image to %s with error %s", resourcePath.c_str(), StringForResourceError(error));
		return ImageResource::Invalid;
	}

	imageMap->insert({ resourcePath,image });

	return image;
}

ImageResource ResourceManager::ImageForPath(const std::string & path)
{
	auto itr = imageMap->find(path);
	if (itr != imageMap->end())
	{
		return itr->second;
	}

	ImageResource image;
	image.DefaultConstruct();

	int err = image->LoadFromFile(path);
	if (err != RESOURCE_ERROR_NO_ERROR)
	{
		Log(LogLevel_Info, "Error loading Image for path %s with error %s", path.c_str(), StringForResourceError(err));
		return ImageResource::Invalid;
	}
	return image;
}

SoundResource ResourceManager::MakeSoundForPath(const std::string & source, const std::string & resourcePath)
{
	SoundResource sound;
	sound.DefaultConstruct();

	int error = sound->MakeFromFile(source);
	if (error != RESOURCE_ERROR_NO_ERROR)
	{
		Log(LogLevel_Error, "Error Loading sound from %s with error %s", source.c_str(), StringForResourceError(error));
		return SoundResource::Invalid;
	}
	error = sound->SaveToFile(resourcePath);
	if (error != RESOURCE_ERROR_NO_ERROR)
	{
		Log(LogLevel_Error, "Error Saving sound to %s with error %s", resourcePath.c_str(), StringForResourceError(error));
		return SoundResource::Invalid;
	}

	soundMap->insert({ resourcePath,sound });

	return sound;
}

SoundResource ResourceManager::SoundForPath(const std::string & path)
{
	auto itr = soundMap->find(path);
	if (itr != soundMap->end())
	{
		return itr->second;
	}

	SoundResource sound;
	sound.DefaultConstruct();

	int err = sound->LoadFromFile(path);
	if (err != RESOURCE_ERROR_NO_ERROR)
	{
		Log(LogLevel_Info, "Error loading sound for path %s with error %s", path.c_str(), StringForResourceError(err));
		return SoundResource::Invalid;
	}
	return sound;
}

ModelResource ResourceManager::MakeModelForPath(const std::string & source, const std::string & resourcePath)
{
	ModelResource model;
	model.DefaultConstruct();

	int error = model->MakeFromFile(source);
	if (error != RESOURCE_ERROR_NO_ERROR)
	{
		Log(LogLevel_Error, "Error Loading model from %s with error %s", source.c_str(), StringForResourceError(error));
		return ModelResource::Invalid;
	}
	error = model->SaveToFile(resourcePath);
	if (error != RESOURCE_ERROR_NO_ERROR)
	{
		Log(LogLevel_Error, "Error Saving model to %s with error %s", resourcePath.c_str(), StringForResourceError(error));
		return ModelResource::Invalid;
	}

	modelMap->insert({ resourcePath,model });

	return model;
}

ModelResource ResourceManager::ModelForPath(const std::string & path)
{
	auto itr = modelMap->find(path);
	if (itr != modelMap->end())
	{
		return itr->second;
	}

	ModelResource model;
	model.DefaultConstruct();

	int err = model->LoadFromFile(path);
	if (err != RESOURCE_ERROR_NO_ERROR)
	{
		Log(LogLevel_Info, "Error loading sound for path %s with error %s", path.c_str(), StringForResourceError(err));
		return ModelResource::Invalid;
	}
	return model;
}

void ResourceManager::CleanupResources()
{
	// fonts
	for (auto it = fontMap->cbegin(); it != fontMap->cend();)
	{
		if (it->second.GetRetainCount() == 1)
		{
			it = fontMap->erase(it);
		}
		else
		{
			++it;
		}
	}
	// images
	for (auto it = imageMap->cbegin(); it != imageMap->cend();)
	{
		if (it->second.GetRetainCount() == 1)
		{
			it = imageMap->erase(it);
		}
		else
		{
			++it;
		}
	}
	// sounds
	for (auto it = soundMap->cbegin(); it != soundMap->cend();)
	{
		if (it->second.GetRetainCount() == 1)
		{
			it = soundMap->erase(it);
		}
		else
		{
			++it;
		}
	}
	// models
	for (auto it = modelMap->cbegin(); it != modelMap->cend();)
	{
		if (it->second.GetRetainCount() == 1)
		{
			it = modelMap->erase(it);
		}
		else
		{
			++it;
		}
	}
}

ResourceManager * ResourceManager::GetResourceManager()
{
	if (instance == 0)instance = new ResourceManager;
	return instance;
}
