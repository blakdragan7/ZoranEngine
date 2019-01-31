#include "stdafx.h"
#include "ImageAsset.h"
#include "ResourceErrors.h"

#include <Rendering/RenderEngineBase.h>
#include <Rendering/TextureBase.h>
#include <Utils/StringAddons.hpp>

#include <ThirdParty/loadpng/lodepng.h>

#include <iostream>
#include <fstream>

const std::string ziaHeader = "zia_version_1_0";
const std::string sourceHeader = "zia_source";
const std::string imageHeader = "zia_image";
const std::string typeHeader = "zia_image_type";

ImageAsset::ImageAsset(): renderAsset(0)
{
	sourceFile = new std::string;
	zSourcePath = new std::string;
}

ImageAsset::ImageAsset(TextureBase * renderAsset) : renderAsset(renderAsset)
{
	sourceFile = new std::string;
	zSourcePath = new std::string;
}

ImageAsset::~ImageAsset()
{
	if(renderAsset)delete renderAsset;
	delete sourceFile;
	delete zSourcePath;
}

int ImageAsset::MakeFromFile(const std::string & file, RenderDataType type, RenderDataFormat format)
{
	int error = 0;
	renderAsset = rEngine->CreateTexture(file.c_str(), type, format, &error);

	if (renderAsset == 0)
	{
		return RESOURCE_ERROR_UNKOWN_ERROR;
	}

	isLoaded = true;

	*sourceFile = file;

	return RESOURCE_ERROR_NO_ERROR;
}

int ImageAsset::LoadFromFile(const std::string & file)
{
	if (isLoaded == true)
	{
		return RESOURCE_ERROR_ALREADY_LOADED;
	}

	std::string filePath = file;

	if (GetStringFileType(StringToLower(filePath)) != "zia")
	{
		filePath += ".zia";
	}

	std::fstream fileS(filePath, std::ios::in | std::ios::binary);

	if (fileS.good())
	{
		std::string currentHeader;
		std::getline(fileS, currentHeader, '\n');

		if (currentHeader != ziaHeader)
		{
			Log(LogLevel_Error, "File %s Does not Contain Correct Header !!", filePath.c_str());

			return RESOURCE_ERROR_LOADING_FILE;
		}

		unsigned foundHeaders = 0;

		while (fileS.eof() == false)
		{
			currentHeader.clear();
			std::getline(fileS, currentHeader);

			if (currentHeader.empty())
				continue;

			if (currentHeader == imageHeader)
			{
				RenderDataFormat format;
				RenderDataType type;
				size_t dataSize = 0;
				char* encodedData = 0;
				unsigned char* data = 0;

				fileS.read((char*)&format, sizeof(RenderDataFormat));
				fileS.read((char*)&type, sizeof(RenderDataType));
				fileS.read((char*)&dataSize, sizeof(dataSize));

				encodedData = (char*)malloc(dataSize);

				fileS.read(encodedData, dataSize);

				unsigned width,height;

				lodepng_decode_memory(&data, &width, &height, (const unsigned char*)encodedData, dataSize, LodePNGColorType::LCT_RGBA, 8);

				renderAsset = rEngine->CreateTexture(data, type, format, { (int)width, (int)height });

				free(data);
				free(encodedData);
			}
			if (currentHeader == sourceHeader)
			{
				std::getline(fileS, *sourceFile);
			}

			foundHeaders++;
		}

		if (foundHeaders !=2)
		{
			Log(LogLevel_Error, "Malformed asset file while loading %s", filePath.c_str());
			return RESOURCE_ERROR_LOADING_FILE;
		}

		fileS.close();
	}
	else
	{
		char message[256] = { 0 };
		strerror_s(message, errno);
		Log(LogLevel_Error, "Could not open file for reading, %s, error %s", filePath.c_str(), message);
		return RESOURCE_ERROR_LOADING_FILE;
	}

	isLoaded = true;

	*zSourcePath = filePath;
	return RESOURCE_ERROR_NO_ERROR;
}

int ImageAsset::SaveToFile(const std::string & file)
{
	if (isLoaded == false)
	{
		return RESOURCE_ERROR_NOT_LOADED;
	}

	std::string filePath = file;

	if (GetStringFileType(StringToLower(filePath)) != "zia")
	{
		filePath += ".zia";
	}

	std::fstream fileS(filePath, std::ios::out | std::ios::binary | std::ios::trunc);

	if (fileS.good())
	{
		fileS << ziaHeader + "\n";
		
		fileS << sourceHeader + "\n";
		fileS << *sourceFile + "\n";

		fileS << imageHeader + "\n";
		
		size_t dataSize;
		char* tData = 0;
		if (renderAsset->GetTextureData(&tData, dataSize) == false)
		{
			fileS.close();
			Log(LogLevel_Error, "Could not read data from texture !");
			return RESOURCE_ERROR_SAVING_FILE;
		}
		
		RenderDataFormat format = renderAsset->GetRenderDataFormat();
		RenderDataType type = renderAsset->GetRenderDataType();
		Vector2I size = renderAsset->GetSize();

		fileS.write((char*)&format, sizeof(RenderDataFormat));
		fileS.write((char*)&type, sizeof(RenderDataType));

		unsigned char*  encodedData = 0;
		size_t encodedSize = 0;

		lodepng_encode_memory(&encodedData, &encodedSize, (unsigned char*)tData, (unsigned)size.w, (unsigned)size.h, LCT_RGBA, 8);

		fileS.write((const char*)&dataSize, sizeof(dataSize));
		fileS.write((const char*)encodedData, encodedSize);

		free(tData);

		fileS.close();
	}
	else
	{
		char message[256] = { 0 };
		strerror_s(message,errno);
		Log(LogLevel_Error, "Could not open file for writing, %s, error %s", filePath.c_str(), message);

		return RESOURCE_ERROR_SAVING_FILE;
	}

	return RESOURCE_ERROR_NO_ERROR;
}

const char * ImageAsset::GetAssetDescription() const
{
	return "ImageAsset - represents a loaded image file (like png) from disk that can be used to as a texture in rendering";
}
