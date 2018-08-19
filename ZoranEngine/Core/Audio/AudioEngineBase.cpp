#include "stdafx.h"
#include "AudioEngineBase.h"
#include "Common.h"

AudioEngineBase::~AudioEngineBase()
{
	for (auto iter : audioDevices)
	{
		AudioDevice* device = iter;
		delete device;
	}

	for (auto iter : audioListeners)
	{
		AudioListener* listener = iter;
		delete listener;
	}

	for (auto iter : soundInstances)
	{
		SoundInstance* sound = iter;
		delete sound;
	}

	audioDevices.clear();
	audioListeners.clear();
	soundInstances.clear();
}

const char * AudioEngineBase::StringForError(AudioError error)
{
	switch (error)
	{
	case AE_NO_ERROR:
		return "No Error";
		break;
	case AE_NOT_SUPPORTED:
		return "Not Supported";
		break;
	case AE_INVALID_OPERATION:
		return "Invalid Operation";
		break;
	case AE_NOT_INITIALIZED:
		return "Not initialized";
		break;
	default:
		return 0;
	}
}
