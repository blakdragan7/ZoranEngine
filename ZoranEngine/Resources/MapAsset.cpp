#include "stdafx.h"
#include "MapAsset.h"

#include "ResourceErrors.h"

MapAsset::MapAsset()
{
	levelObjects = new std::vector<SceneObject*>;
	filePath = new std::string;
}


MapAsset::~MapAsset()
{
	delete levelObjects;
	delete filePath;
}

int MapAsset::LoadFromFile(const std::string & file)
{
	return RESOURCE_ERROR_NOT_SUPPORTED;
}

int MapAsset::SaveToFile(const std::string & file)
{
	return RESOURCE_ERROR_NOT_SUPPORTED;
}

int MapAsset::Save()
{
	return RESOURCE_ERROR_NOT_SUPPORTED;
}
