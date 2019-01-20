#include "stdafx.h"
#include "ModelAsset.h"


ModelAsset::ModelAsset()
{
}


ModelAsset::~ModelAsset()
{
}

int ModelAsset::LoadFromFile(const std::string & file)
{
	return 0;
}

int ModelAsset::SaveToFile(const std::string & file)
{
	return 0;
}

void ModelAsset::DestroyAsset()
{
}

const char * ModelAsset::GetAssetDescription() const
{
	return "ModelAsset - represents a loaded Model asset (like .fbx) that can be rendered";
}
