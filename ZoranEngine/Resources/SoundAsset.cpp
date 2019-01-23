#include "stdafx.h"
#include "SoundAsset.h"

#include <Audio/SoundInstance.h>
#include <Audio/AudioEngineBase.h>

#include <Resources/ResourceErrors.h>

SoundAsset::SoundAsset() : playableAsset(0), listener(0)
{

}

SoundAsset::SoundAsset(AudioListener* listener) : playableAsset(0), listener(listener)
{

}

SoundAsset::~SoundAsset()
{
	if (playableAsset)
	{
		aEngine->DestroySound(playableAsset);
	}
}

void SoundAsset::AssignListener(AudioListener * listener)
{
	this->listener = listener;
}

void SoundAsset::Play()
{
	if (listener)
	{
		aEngine->MakeListenerActive(listener);
		aEngine->PlaySoundInstance(playableAsset);
	}
	else
	{
		Log(LogLevel_Error, "Trying to play sound without a listener !");
		return;
	}
}

void SoundAsset::PlayAtLocation(Vec3D location)
{
	if (listener)
	{
		aEngine->MakeListenerActive(listener);
		aEngine->PlaySoundAtLocation(playableAsset,location);
	}
	else
	{
		Log(LogLevel_Error, "Trying to play sound without a listener !");
		return;
	}
}

void SoundAsset::PlayAtLocation(Vec2D location)
{
	if (listener)
	{
		aEngine->MakeListenerActive(listener);
		aEngine->PlaySoundAtLocation(playableAsset, location);
	}
	else
	{
		Log(LogLevel_Error, "Trying to play sound without a listener !");
		return;
	}
}

void SoundAsset::Stop()
{
	if (listener)
	{
		aEngine->MakeListenerActive(listener);
		aEngine->StopSound(playableAsset);
	}
	else
	{
		Log(LogLevel_Error, "Trying to stop sound without a listener !");
		return;
	}
}

int SoundAsset::MakeFromFile(const std::string & file)
{
	return RESOURCE_ERROR_NOT_SUPPORTED;
}

int SoundAsset::LoadFromFile(const std::string & file)
{
	return RESOURCE_ERROR_NOT_SUPPORTED;
}

int SoundAsset::SaveToFile(const std::string & file)
{
	return RESOURCE_ERROR_NOT_SUPPORTED;
}

const char * SoundAsset::GetAssetDescription() const
{
	return "SoundAsset - represents a loaded sound asset (like .wav) that can be played";
}
