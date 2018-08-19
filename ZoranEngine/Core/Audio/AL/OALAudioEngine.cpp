#include "stdafx.h"
#include "OALAudioEngine.h"
#include <ThirdParty/AL/alc.h>
#include <ThirdParty/AL/al.h>
#include <iostream>

#include <Core/Audio/Common.h>
#include "OALImplementations.h"

#include <assert.h>

OALAudioEngine::OALAudioEngine()
{
	if (alcIsExtensionPresent(NULL,"ALC_ENUMERATION_EXT") == AL_TRUE)
	{
		const char* devices = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
		const char* device = devices;
		size_t len = 0;
		while (device && *device != '\0') {
			std::cout << device << std::endl;
			len = strlen(device);
			device += (len + 2);
			OALAudioDevice* aDevice = new OALAudioDevice(device);
			audioDevices.push_back(aDevice);
		}
	}
}

OALAudioEngine::~OALAudioEngine()
{
	
}

AudioError OALAudioEngine::Init(AudioDevice * toDevice)
{
	assert(toDevice->type != AEI_INVALID && "Device Type not set in implementation !");
	if (toDevice->type != AEI_OAL)return AE_INVALID_AUDIO_DEVICE;
	OALAudioDevice* oalDevice = (OALAudioDevice*)toDevice;
	if(oalDevice->oalDevice != 0)return AE_INVALID_AUDIO_DEVICE;
	ALCdevice* alcDevice = 0;
	if (toDevice)
	{
		alcDevice = alcOpenDevice(toDevice->deviceDescription.c_str());
		if (alcDevice == 0)return AE_INVALID_OPERATION;
		oalDevice->oalDevice = alcDevice;
	}
	else // Open Default Device
	{
		alcDevice = alcOpenDevice("");
		if (alcDevice == 0)return AE_INVALID_OPERATION;
		oalDevice->oalDevice = alcDevice;
	}

	currentDevice = oalDevice;

	return AE_NO_ERROR;
}

AudioError OALAudioEngine::SwitchToDevice(AudioDevice * toDevice, AudioDevice * fromDevice)
{
	assert(toDevice->type != AEI_INVALID && fromDevice->type != AEI_INVALID && "Device Type not set in implementation !");
	if (toDevice->type != AEI_OAL || fromDevice->type != AEI_OAL)return AE_INVALID_AUDIO_DEVICE;

	return AE_NO_ERROR;
}

AudioError OALAudioEngine::CreateAudioListener(const Vector3D & location, AudioListener ** outListener)
{
	OALAudioListener* listener = new OALAudioListener();

	listener->context = alcCreateContext(currentDevice->oalDevice, 0);

	

	return AE_NO_ERROR;
}

AudioError OALAudioEngine::CreateAudioListener(const Vector2D & location, AudioListener ** outListener)
{
	return AE_NO_ERROR;
}

AudioError OALAudioEngine::DestroyListener(AudioListener * listener)
{
	return AudioError();
}

AudioError OALAudioEngine::SetListener(AudioListener * listener)
{
	return AudioError();
}

AudioError OALAudioEngine::PlaySoundAtLocation(SoundInstance * sound, const Vector3D & location)
{
	return AudioError();
}

AudioError OALAudioEngine::PlaySoundAtLocation(SoundInstance * sound, const Vector2D & location)
{
	return AudioError();
}

AudioError OALAudioEngine::PlaySoundInstance(SoundInstance * sound)
{
	return AudioError();
}

void OALAudioEngine::StopSound(SoundInstance * sound)
{
}

bool OALAudioEngine::QuerryCapability(AudioCapability cap)
{
	return false;
}

AudioError OALAudioEngine::SetCapability(AudioCapability cap)
{
	return AE_NOT_SUPPORTED;
}

AudioError OALAudioEngine::SetCapability(std::initializer_list<enum AudioCapability> list)
{
	return AE_NOT_SUPPORTED;
}

AudioError OALAudioEngine::DestroySound(SoundInstance * sound)
{
	return AudioError();
}

const char * OALAudioEngine::StringForError(AudioError error)
{
	return AudioEngineBase::StringForError(error);
}

AudioError OALAudioEngine::CreateSoundFromBuffer(void * buffer, AudioBufferType bType, SoundInstance ** outSound)
{
	return AudioError();
}

AudioError OALAudioEngine::CreateSoundFromFile(const char * file, AudioFileType type, SoundInstance ** outSound)
{
	return AudioError();
}
