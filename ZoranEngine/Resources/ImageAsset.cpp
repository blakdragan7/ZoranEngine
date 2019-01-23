#include "stdafx.h"
#include "ImageAsset.h"
#include "ResourceErrors.h"

#include <Rendering/RenderEngineBase.h>
#include <Rendering/TextureBase.h>
#include <Utils/StringAddons.hpp>

#include <iostream>
#include <fstream>

const std::string ziaHeader = "zia_version_1_0";
const std::string sourceHeader = "zia_source";
const std::string imageHeader = "zia_image";
const std::string typeHeader = "zia_image_type";

ImageAsset::ImageAsset(): renderAsset(0)
{
	sourceFile = new std::string;
}

ImageAsset::ImageAsset(TextureBase * renderAsset) : renderAsset(renderAsset)
{
	sourceFile = new std::string;
}

ImageAsset::~ImageAsset()
{
	if(renderAsset)delete renderAsset;
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

	sourceFile = new std::string(file);

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

			return RESOURCE_ERROR_ERROR_LOADING_FILE;
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
				Vector2I size;
				size_t dataSize = 0;
				char* data = 0;

				fileS.read((char*)&format, sizeof(RenderDataFormat));
				fileS.read((char*)&type, sizeof(RenderDataType));
				fileS.read((char*)&size, sizeof(Vector2I));
				fileS.read((char*)&dataSize, sizeof(dataSize));

				data = (char*)malloc(dataSize);

				fileS.read(data, dataSize);

				renderAsset = rEngine->CreateTexture(data, type, format, size);

				free(data);
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
			return RESOURCE_ERROR_ERROR_LOADING_FILE;
		}

		fileS.close();
	}
	else
	{
		char message[256] = { 0 };
		strerror_s(message, errno);
		Log(LogLevel_Error, "Could not open file for reading, %s, error %s", filePath.c_str(), message);
		return RESOURCE_ERROR_ERROR_LOADING_FILE;
	}

	isLoaded = true;

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

	std::fstream fileS(filePath, std::ios::out | std::ios::binary);

	if (fileS.good())
	{
		std::string data;

		data = ziaHeader + "\n";
		
		data += sourceHeader + "\n";
		data += *sourceFile + "\n";

		data += imageHeader + "\n";
		
		size_t dataSize;
		char* tData = 0;
		if (renderAsset->GetTextureData(&tData, dataSize) == false)
		{
			fileS.close();
			Log(LogLevel_Error, "Could not read data from texture !");
			return RESOURCE_ERROR_ERROR_SAVING_FILE;
		}
		
		RenderDataFormat format = renderAsset->GetRenderDataFormat();
		RenderDataType type = renderAsset->GetRenderDataType();
		Vector2I size = renderAsset->GetSize();

		data.append((char*)&format, sizeof(RenderDataFormat));
		data.append((char*)&type, sizeof(RenderDataType));
		data.append((char*)&size, sizeof(Vector2I));

		//TODO: encode somehow instead of raw data

		data.append((const char*)&dataSize, sizeof(dataSize));
		data.append(tData, dataSize);

		free(tData);

		fileS.write(data.c_str(), data.size());
		fileS.close();
	}
	else
	{
		char message[256] = { 0 };
		strerror_s(message,errno);
		Log(LogLevel_Error, "Could not open file for writing, %s, error %s", filePath.c_str(), message);

		return RESOURCE_ERROR_ERROR_SAVING_FILE;
	}

	return RESOURCE_ERROR_NO_ERROR;
}

const char * ImageAsset::GetAssetDescription() const
{
	return "ImageAsset - represents a loaded image file (like png) from disk that can be used to as a texture in rendering";
}
