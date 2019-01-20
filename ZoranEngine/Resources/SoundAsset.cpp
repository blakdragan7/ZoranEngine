#include "stdafx.h"
#include "SoundAsset.h"


SoundAsset::SoundAsset()
{
}


SoundAsset::~SoundAsset()
{
}

int SoundAsset::LoadFromFile(const std::string & file)
{
	return 0;
}

int SoundAsset::SaveToFile(const std::string & file)
{
	return 0;
}

void SoundAsset::DestroyAsset()
{
}

const char * SoundAsset::GetAssetDescription() const
{
	return "SoundAsset - represents a loaded sound asset (like .wav) that can be played";
}
