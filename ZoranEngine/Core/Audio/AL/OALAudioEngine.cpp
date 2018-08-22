#include "stdafx.h"
#include "OALAudioEngine.h"
#include <ThirdParty/AL/alc.h>
#include <ThirdParty/AL/al.h>
#include <iostream>

#include <Core/Audio/Common.h>
#include "OALImplementations.h"

#include <Core/2D/SceneObject2D.h>
#include <Core/3D/SceneObject3D.h>

#include <assert.h>

AudioError OALAudioEngine::ALCenumToALError(int error)
{
	switch (error)
	{
	case AL_NO_ERROR:
		return AE_NO_ERROR;
		break;
	case AL_INVALID_NAME:
		return AE_BACKEND_ERROR;
		break;
	case AL_INVALID_ENUM:
		return AE_BACKEND_ERROR;
		break;
	case AL_INVALID_OPERATION:
		return AE_INVALID_OPERATION;
		break;
	case AL_INVALID_VALUE:
		return AE_BACKEND_ERROR;
		break;
	case AL_OUT_OF_MEMORY:
		return AE_NO_MEMORY;
		break;
	default:
		return AE_UNKOWN_ERROR;
		break;
	}

	return AE_UNKOWN_ERROR;
}

AudioError OALAudioEngine::CheckErrors(const char* func)
{
	ALCenum error = alcGetError(currentDevice->oalDevice);

	if (error != AL_NO_ERROR)
	{
		Log(LogLevel_Default, "%s OpenAL error %s \n", func, alGetString(error) );
	}

	return ALCenumToALError(error);
}

OALAudioEngine::OALAudioEngine() : activeListener(0), currentDevice(0)
{
	if (alcIsExtensionPresent(NULL,"ALC_ENUMERATION_EXT") == AL_TRUE)
	{
		const char* devices = alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER);
		const char* device = devices;
		size_t len = 0;
		while (device && *device != '\0') {
			std::cout << device << std::endl;
			
			OALAudioDevice* aDevice = new OALAudioDevice(device);
			audioDevices.push_back(aDevice);

			len = strlen(device);
			device += (len + 1);
		}
	}
}

OALAudioEngine::~OALAudioEngine()
{
	
}

AudioError OALAudioEngine::Init(AudioDevice * toDevice)
{
	if (toDevice)
	{
		assert(toDevice->type != AEI_INVALID && "Device Type not set in implementation !");
		if (toDevice->type != AEI_OAL)return AE_INVALID_AUDIO_DEVICE;
		OALAudioDevice* oalDevice = (OALAudioDevice*)toDevice;
		if (oalDevice->oalDevice != 0)return AE_INVALID_AUDIO_DEVICE;

		ALCdevice* alcDevice = alcOpenDevice(toDevice->deviceDescription.c_str());
		if (alcDevice == 0)return AE_INVALID_OPERATION;
		oalDevice->oalDevice = alcDevice;
		currentDevice = oalDevice;
	}
	else // Open Default Device
	{
		OALAudioDevice* oalDevice = 0;

		ALCdevice* alcDevice = alcOpenDevice("");
		if (alcDevice == 0)return AE_INVALID_OPERATION;

		const char* deviceName = alcGetString(alcDevice, ALC_ALL_DEVICES_SPECIFIER);
		
		// iterate through devices to find default one
		for (auto odevice : audioDevices)
		{
			if (odevice->deviceDescription == deviceName)
			{
				oalDevice = (OALAudioDevice*)odevice;
				break;
			}
		}

		if (oalDevice == 0)return  AE_NOT_FOUND;
		
		oalDevice->oalDevice = alcDevice;

		currentDevice = oalDevice;
	}

	return AE_NO_ERROR;
}

AudioError OALAudioEngine::SwitchToDevice(AudioDevice * toDevice, AudioDevice * fromDevice)
{
	assert(toDevice->type != AEI_INVALID && fromDevice->type != AEI_INVALID && "Device Type not set in implementation !");
	if (toDevice->type != AEI_OAL || fromDevice->type != AEI_OAL)return AE_INVALID_AUDIO_DEVICE;

	return AE_NO_ERROR;
}

AudioError OALAudioEngine::CreateAudioListener(const SceneObject3D * object, AudioListener ** outListener)
{
	OALAudioListener* listener = new OALAudioListener();

	listener->context = alcCreateContext(currentDevice->oalDevice, 0);

	if (activeListener == 0)
	{
		alcMakeContextCurrent(listener->context);
		activeListener = listener;

		Vec3D location = object->GetPosition();
		Vec3D veloctiy = object->GetVelocity();
		Quaternion orientation = object->GetRotation();

		Vec3D up = orientation.GetUpVector();
		Vec3D forward = orientation.GetForwardVector();

		float alOr[6] = { up.x,up.y,up.z,forward.x,forward.y,forward.z };

		alListener3f(AL_POSITION, location.x, location.y, location.z);
		alListener3f(AL_VELOCITY, veloctiy.x, veloctiy.y, veloctiy.z);
		alListenerfv(AL_ORIENTATION, alOr);
	}


	*outListener = listener;

	return AE_NO_ERROR;
}

AudioError OALAudioEngine::CreateAudioListener(const SceneObject2D * object, AudioListener ** outListener)
{
	OALAudioListener* listener = new OALAudioListener();

	listener->context = alcCreateContext(currentDevice->oalDevice, 0);

	if (activeListener == 0)
	{
		alcMakeContextCurrent(listener->context);
		activeListener = listener;

		Vec2D location = object->GetPosition();
		Vec2D veloctiy = object->GetVelocity();

		alListener3f(AL_POSITION, location.x, location.y, 0);
		alListener3f(AL_VELOCITY, veloctiy.x, veloctiy.y, 0);
	}

	*outListener = listener;

	return AE_NO_ERROR;
}

AudioError OALAudioEngine::MakeListenerActive(AudioListener * listener)
{
	assert(listener->type != AEI_INVALID && "Device Type not set in implementation !");
	if (listener->type != AEI_OAL)return AE_INVALID_AUDIO_DEVICE;

	OALAudioListener* oalListener = (OALAudioListener*)listener;

	if (oalListener->isListening)return AE_NO_ERROR;

	if (alcMakeContextCurrent(oalListener->context) == false)
	{
		return CheckErrors("OALAudioEngine::MakeListenerActive alcMakeContextCurrent");
	}

	oalListener->isListening = true;

	return AE_NO_ERROR;
}

AudioError OALAudioEngine::DestroyListener(AudioListener * listener)
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
