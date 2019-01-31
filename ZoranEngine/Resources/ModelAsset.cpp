#include "stdafx.h"
#include "ModelAsset.h"

#include <fstream>

#include <Utils/StringAddons.hpp>

#include <Resources/ResourceErrors.h>

#include <Rendering/RenderEngineBase.h>
#include <Rendering/Renderers/ModelRenderer.h>

const std::string zmaHeader = "zma_version_1_0";
const std::string sourceHeader = "zma_source";
const std::string modelHeader = "zma_model";
const std::string typeHeader = "zma_model_type";

ModelAsset::ModelAsset() : renderAsset(0), isLoaded(false)
{
	sourcePath = new std::string;
	zSourcePath = new std::string;
}

ModelAsset::ModelAsset(ModelRenderer * renderAsset) : renderAsset(renderAsset), isLoaded(false)
{
	sourcePath = new std::string;
	zSourcePath = new std::string;
}

ModelAsset::~ModelAsset()
{
	delete renderAsset;
	delete sourcePath;
	delete zSourcePath;
}

int ModelAsset::MakeFromFile(const std::string & file)
{
	if (isLoaded)
	{
		return RESOURCE_ERROR_ALREADY_LOADED;
	}

	renderAsset = rEngine->CreateModelRenderer();
	isLoaded = renderAsset->LoadFile(file.c_str());

	if (isLoaded)
	{
		*sourcePath = file;

		return RESOURCE_ERROR_NO_ERROR;
	}
	else
	{
		delete renderAsset;
		return RESOURCE_ERROR_UNKOWN_ERROR;
	}
}

int ModelAsset::LoadFromFile(const std::string & file)
{
	if (isLoaded == true)
	{
		return RESOURCE_ERROR_ALREADY_LOADED;
	}

	std::string filePath = file;

	if (GetStringFileType(StringToLower(filePath)) != "zma")
	{
		filePath += ".zma";
	}

	std::fstream fileS;
	fileS.open(filePath.c_str(), std::ios::binary | std::ios::in);

	if (fileS.good())
	{
		std::string line;
		std::getline(fileS, line);

		if (line != zmaHeader)
		{
			Log(LogLevel_Error, "File %s Does not Contain Correct Header !!", filePath.c_str());
			return RESOURCE_ERROR_LOADING_FILE;
		}

		int numHeaders = 0;

		line.clear();
		while (std::getline(fileS, line))
		{
			if (line.empty())continue;

			if (line == sourceHeader)
			{
				std::getline(fileS, *sourcePath);
			}
			else if (line == modelHeader)
			{
				size_t dataSize;
				fileS.read((char*)&dataSize, sizeof(size_t));

				char* data = new char[dataSize];
				fileS.read(data, dataSize);

				renderAsset = rEngine->CreateModelRenderer();

				if (renderAsset->LoadFileFromMemory(data, dataSize) == false)
				{
					delete renderAsset;
					fileS.close();
					Log(LogLevel_Error, "Error Creating render Asset from loaded memory");
					return RESOURCE_ERROR_LOADING_FILE;
				}

				delete[] data;
			}

			++numHeaders;
			line.clear();
		}

		if (numHeaders != 2)
		{
			Log(LogLevel_Error, "Incorrect Number of Headers for file %s", filePath.c_str());
			return RESOURCE_ERROR_LOADING_FILE;
		}
	}
	else
	{
		Log(LogLevel_Error, "Error Loading Model Asset, Could Not Open %s for Reading", filePath.c_str());
		return RESOURCE_ERROR_LOADING_FILE;
	}

	fileS.close();

	*zSourcePath = filePath;
	isLoaded = true;
	return RESOURCE_ERROR_NO_ERROR;
}

int ModelAsset::SaveToFile(const std::string & file)
{
	if (isLoaded == false)
	{
		return RESOURCE_ERROR_NOT_LOADED;
	}

	std::string filePath = file;

	if (GetStringFileType(StringToLower(filePath)) != "zma")
	{
		filePath += ".zma";
	}

	std::fstream fileS(filePath, std::ios::out | std::ios::binary | std::ios::trunc);

	if (fileS.good())
	{
		std::string data;

		fileS <<zmaHeader + "\n";
		fileS << sourceHeader + "\n";
		fileS << *sourcePath + "\n";
		fileS << modelHeader + "\n";

		std::string modelData;
		if (renderAsset->GetModelAsFBX(modelData) == false)
		{
			Log(LogLevel_Error, "Could Not Get Model Data For Saving !");
			return RESOURCE_ERROR_SAVING_FILE;
		}

		size_t modelSize = modelData.size();

		fileS.write((char*)&modelSize, sizeof(size_t));
		fileS.write(modelData.c_str(), modelSize);

		fileS.close();
	}
	else
	{
		char message[256] = { 0 };
		strerror_s(message, errno);
		Log(LogLevel_Error, "Could not open file for writing, %s, error %s", filePath.c_str(), message);

		return RESOURCE_ERROR_SAVING_FILE;
	}

	*zSourcePath = filePath;

	return RESOURCE_ERROR_NO_ERROR;
}

const char * ModelAsset::GetAssetDescription() const
{
	return "ModelAsset - represents a loaded Model asset (like .fbx) that can be rendered";
}

void ModelAsset::RenderObject(const Matrix44 & matrix)
{
	if(renderAsset)renderAsset->RenderObject(matrix);
}
