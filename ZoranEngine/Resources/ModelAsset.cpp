#include "stdafx.h"
#include "ModelAsset.h"

#include <Resources/ResourceErrors.h>

ModelAsset::ModelAsset()
{
}


ModelAsset::~ModelAsset()
{
}

int ModelAsset::MakeFromFile(const std::string & file)
{
	return RESOURCE_ERROR_NOT_SUPPORTED;
}

int ModelAsset::LoadFromFile(const std::string & file)
{
	return RESOURCE_ERROR_NOT_SUPPORTED;
}

int ModelAsset::SaveToFile(const std::string & file)
{
	return RESOURCE_ERROR_NOT_SUPPORTED;
}

const char * ModelAsset::GetAssetDescription() const
{
	return "ModelAsset - represents a loaded Model asset (like .fbx) that can be rendered";
}
