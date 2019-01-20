#include "stdafx.h"
#include "ImageAsset.h"
#include "ResourceErrors.h"

#include <Rendering/RenderEngineBase.h>
#include <Rendering/TextureBase.h>

ImageAsset::ImageAsset(): renderAsset(0)
{
}

ImageAsset::ImageAsset(TextureBase * renderAsset) : renderAsset(renderAsset)
{
}

ImageAsset::~ImageAsset()
{
	if(renderAsset)delete renderAsset;
}

int ImageAsset::LoadFromFile(const std::string & file, RenderDataType type, RenderDataFormat format)
{
	int error = 0;
	renderAsset = rEngine->CreateTexture(file.c_str(), type, format, &error);

	if (renderAsset == 0)
	{
		return RESOURCE_ERROR_UNKOWN_ERROR;
	}

	isLoaded = true;

	return RESOURCE_ERROR_NO_ERROR;
}

int ImageAsset::LoadFromFile(const std::string & file)
{
	int error = 0;
	renderAsset = rEngine->CreateTexture(file.c_str(), Render_Data_Type_RGBA_32, Render_Data_Format_Unsigned_Byte, &error);

	if (renderAsset == 0)
	{
		return RESOURCE_ERROR_UNKOWN_ERROR;
	}

	isLoaded = true;

	return RESOURCE_ERROR_NO_ERROR;
}

int ImageAsset::SaveToFile(const std::string & file)
{
	Log(LogLevel_Warning, "Re-Saving Image Assets is currently not supported");
	return RESOURCE_ERROR_NOT_SUPPORTED;
}

void ImageAsset::DestroyAsset()
{
	if (renderAsset)delete renderAsset;
}

const char * ImageAsset::GetAssetDescription() const
{
	return "ImageAsset - represents a loaded image file (like png) from disk that can be used to as a texture in rendering";
}
