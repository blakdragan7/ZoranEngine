#include "stdafx.h"
#include "OALAudioEngine.h"
#include <ThirdParty/AL/al.h>
#include <ThirdParty/AL/alc.h>
#include <ThirdParty/AL/alut.h>
#include <iostream>

#include <Core/Audio/Common.h>
#include "OALImplementations.h"

#include <Core/2D/SceneObject2D.h>
#include <Core/3D/SceneObject3D.h>

#include <Utils/VectorAddons.hpp>

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

AudioError OALAudioEngine::ALUTenumToALError(int error)
{
	switch (error)
	{
	case ALUT_ERROR_NO_ERROR:
		return AE_NO_ERROR;
		break;
	case ALUT_ERROR_INVALID_ENUM:
		return AE_BACKEND_ERROR;
		break;
	case ALUT_ERROR_BUFFER_DATA:
		return AE_BACKEND_ERROR;
		break;
	case ALUT_ERROR_INVALID_OPERATION:
		return AE_INVALID_OPERATION;
		break;
	case ALUT_ERROR_INVALID_VALUE:
		return AE_BACKEND_ERROR;
		break;
	case ALUT_ERROR_OUT_OF_MEMORY:
		return AE_NO_MEMORY;
		break;
	case ALUT_ERROR_CREATE_CONTEXT:
		return AE_BACKEND_ERROR;
		break;
	case ALUT_ERROR_NO_CURRENT_CONTEXT:
		return AE_BACKEND_ERROR;
		break;
	case ALUT_ERROR_AL_ERROR_ON_ENTRY:
		return AE_BACKEND_ERROR;
		break;
	case ALUT_ERROR_ALC_ERROR_ON_ENTRY:
		return AE_BACKEND_ERROR;
		break;
	case ALUT_ERROR_OPEN_DEVICE:
		return AE_BACKEND_ERROR;
		break;
	case ALUT_ERROR_CLOSE_DEVICE:
		return AE_BACKEND_ERROR;
		break;
	case ALUT_ERROR_MAKE_CONTEXT_CURRENT:
		return AE_BACKEND_ERROR;
		break;
	case ALUT_ERROR_DESTROY_CONTEXT:
		return AE_BACKEND_ERROR;
		break;
	case ALUT_ERROR_GEN_BUFFERS:
		return AE_BACKEND_ERROR;
		break;
	case ALUT_ERROR_IO_ERROR:
		return AE_BACKEND_ERROR;
		break;
	case ALUT_ERROR_UNSUPPORTED_FILE_TYPE:
		return AE_NOT_SUPPORTED;
		break;
	case ALUT_ERROR_UNSUPPORTED_FILE_SUBTYPE:
		return AE_NOT_SUPPORTED;
		break;
	case ALUT_ERROR_CORRUPT_OR_TRUNCATED_DATA:
		return AE_BACKEND_ERROR;
		break;
	default:
		return AE_UNKOWN_ERROR;
		break;
	}

	return AE_UNKOWN_ERROR;
}

unsigned OALAudioEngine::AEBufferTypeToOAL(AudioBufferType bufferType)
{
	switch (bufferType)
	{
	case ABT_LPCM_MONO_8:
		return AL_FORMAT_MONO8;
		break;
	case ABT_LPCM_MONO_16:
		return AL_FORMAT_MONO16;
		break;
	case ABT_LPCM_STEREO_8:
		return AL_FORMAT_STEREO8;
		break;
	case ABT_LPCM_STEREO_16:
		return AL_FORMAT_STEREO16;
		break;
	default:
		return 0;
	};

	return 0;
}

AudioError OALAudioEngine::CheckALErrors(const char* func)
{
	ALCenum error = alcGetError(currentDevice->oalDevice);

	if (error != AL_NO_ERROR)
	{
		Log(LogLevel_Default, "%s OpenAL error %s \n", func, alGetString(error) );
	}

	return ALCenumToALError(error);
}

AudioError OALAudioEngine::CheckALUTErrors(const char * func)
{
	ALCenum error = alutGetError();

	if (error != ALUT_ERROR_NO_ERROR)
	{
		Log(LogLevel_Default, "%s OpenAL error %s \n", func, alutGetErrorString(error));
	}

	return ALUTenumToALError(error);
}

unsigned OALAudioEngine::AECapabilityToOAL(AudioCapability cap)
{
	// TODO implement this
	switch (cap)
	{

	default:
		return AE_NOT_FOUND;
	}
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

	if (alutInitWithoutContext(0, 0) == false)
	{
		return CheckALUTErrors("OALAudioEngine::Init alutInitWithoutContext");
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


	audioListeners.push_back(listener);

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

	audioListeners.push_back(listener);

	*outListener = listener;

	return AE_NO_ERROR;
}

AudioError OALAudioEngine::MakeListenerActive(AudioListener * listener)
{
	assert(listener->type != AEI_INVALID && "AudioListener Type not set in implementation !");
	if (listener->type != AEI_OAL)return AE_INVALID_AUDIO_DEVICE;

	OALAudioListener* oalListener = (OALAudioListener*)listener;

	if (oalListener->isListening)return AE_NO_ERROR;

	if (alcMakeContextCurrent(oalListener->context) == false)
	{
		return CheckALErrors("OALAudioEngine::MakeListenerActive alcMakeContextCurrent");
	}

	oalListener->isListening = true;

	return AE_NO_ERROR;
}

AudioError OALAudioEngine::DestroyListener(AudioListener * listener)
{
	assert(listener->type != AEI_INVALID && "AudioListener Type not set in implementation !");
	if (listener->type != AEI_OAL)return AE_INVALID_AUDIO_DEVICE;

	OALAudioListener* oalListener = (OALAudioListener*)listener;

	if (oalListener->isListening)
	{
		alcMakeContextCurrent(NULL);
		alcDestroyContext(oalListener->context);
	}

	audioListeners.remove(listener);

	delete oalListener;

	return AudioError();
}

AudioError OALAudioEngine::PlaySoundAtLocation(SoundInstance * sound, const Vector3D & location)
{
	assert(sound->type != AEI_INVALID && "SoundInstance Type not set in implementation !");
	if (sound->type != AEI_OAL)return AE_INVALID_SOUND_INSTANCE;

	OALSoundInstace* oal = (OALSoundInstace*)sound;

	alSource3f(oal->source, AL_POSITION, location.x, location.y, location.z);

	alSourcePlay(oal->source);

	return AE_NO_ERROR;
}

AudioError OALAudioEngine::PlaySoundAtLocation(SoundInstance * sound, const Vector2D & location)
{
	assert(sound->type != AEI_INVALID && "SoundInstance Type not set in implementation !");
	if (sound->type != AEI_OAL)return AE_INVALID_SOUND_INSTANCE;

	OALSoundInstace* oal = (OALSoundInstace*)sound;

	alSource3f(oal->source, AL_POSITION, location.x, location.y, 0.0f);

	alSourcePlay(oal->source);

	return AE_NO_ERROR;
}

AudioError OALAudioEngine::PlaySoundInstance(SoundInstance * sound)
{
	assert(sound->type != AEI_INVALID && "SoundInstance Type not set in implementation !");
	if (sound->type != AEI_OAL)return AE_INVALID_SOUND_INSTANCE;

	OALSoundInstace* oal = (OALSoundInstace*)sound;

	alSourcePlay(oal->source);

	return AE_NO_ERROR;
}

AudioError OALAudioEngine::StopSound(SoundInstance * sound)
{
	assert(sound->type != AEI_INVALID && "SoundInstance Type not set in implementation !");
	if (sound->type != AEI_OAL)return AE_INVALID_SOUND_INSTANCE;

	OALSoundInstace* oal = (OALSoundInstace*)sound;

	alSourceStop(oal->source);

	return AE_NO_ERROR;
}

bool OALAudioEngine::QuerryCapability(AudioCapability cap)
{
	//TODO implement the rest of the querries
	if (cap == AC_DOPPLER)return true;
	return false;
}

AudioError OALAudioEngine::SetCapability(AudioCapability cap)
{
	//TODO implement thre rest of these effects
	if (cap == AC_DOPPLER)return AE_NO_ERROR;
	return AE_NOT_SUPPORTED;
}

AudioError OALAudioEngine::SetCapability(std::initializer_list<enum AudioCapability> list)
{
	for (auto capability : list)
	{
		if (AudioError error = SetCapability(capability) != AE_NO_ERROR)
		{
			return error;
		}
	}

	return AE_NO_ERROR;
}

AudioError OALAudioEngine::DestroySound(SoundInstance * sound)
{
	assert(sound->type != AEI_INVALID && "SoundInstance Type not set in implementation !");
	if (sound->type != AEI_OAL)return AE_INVALID_SOUND_INSTANCE;

	OALSoundInstace* oalSound = (OALSoundInstace*)sound;

	alDeleteSources(1, &oalSound->source);
	alDeleteBuffers(1, &oalSound->buffer);

	soundInstances.remove(sound);

	delete oalSound;

	return AE_NO_ERROR;
}

const char * OALAudioEngine::StringForError(AudioError error)
{
	return AudioEngineBase::StringForError(error);
}

AudioError OALAudioEngine::CreateSoundFromBuffer(void * buffer, unsigned buffer_len, unsigned freq, AudioBufferType bType, SoundInstance ** outSound)
{
	if (buffer == 0 || buffer_len == 0) return AE_INVALID_OPERATION;

	ALenum format = (ALenum)AEBufferTypeToOAL(bType);

	if (format == 0)
	{
		return AE_NOT_SUPPORTED;
	}

	OALSoundInstace* instance = new OALSoundInstace();
	instance->bufferSize = buffer_len;

	alGenBuffers(1, &instance->buffer);

	auto error = CheckALErrors("OALAudioEngine::CreateSoundFromBuffer alGenBuffers");

	if (error != AE_NO_ERROR)
	{
		delete instance;
		return error;
	}

	alBufferData(instance->buffer, format, buffer, buffer_len, freq);

	error = CheckALErrors("OALAudioEngine::CreateSoundFromBuffer alBufferData");

	if (error != AE_NO_ERROR)
	{
		alDeleteBuffers(1, &instance->buffer);
		delete instance;
		return error;
	}

	alGenSources(1, &instance->source);
	
	error = CheckALErrors("OALAudioEngine::CreateSoundFromBuffer alGenSources");

	if (error != AE_NO_ERROR)
	{
		alDeleteBuffers(1, &instance->buffer);
		delete instance;
		return error;
	}

	alSourcei(instance->source, AL_BUFFER, instance->buffer);

	error = CheckALErrors("OALAudioEngine::CreateSoundFromBuffer alSourcei");

	if (error != AE_NO_ERROR)
	{
		alDeleteSources(1, &instance->source);
		alDeleteBuffers(1, &instance->buffer);
		delete instance;
		return error;
	}

	soundInstances.push_back(instance);

	*outSound = instance;

	return AE_NO_ERROR;
}

AudioError OALAudioEngine::CreateSoundFromFile(const char * file, AudioFileType type, SoundInstance ** outSound)
{
	if (file == 0) return AE_INVALID_OPERATION;
	if (type != AFT_WAV) return AE_NOT_SUPPORTED;


	ALenum format = 0;
	ALuint buffer_len, freq;
	buffer_len = freq = 0;
	void* buffer = 0;

	OALSoundInstace* instance = new OALSoundInstace;

	instance->buffer = alutCreateBufferFromFile(file);

	AudioError error = CheckALUTErrors("OALAudioEngine::CreateSoundFromBuffer alutCreateBufferFromFile");

	if (error != AE_NO_ERROR)
	{
		delete instance;
		return error;
	}
	alGenSources(1, &instance->source);

	error = CheckALErrors("OALAudioEngine::CreateSoundFromBuffer alGenSources");

	if (error != AE_NO_ERROR)
	{
		alDeleteBuffers(1, &instance->buffer);
		delete instance;
		return error;
	}

	alSourcei(instance->source, AL_BUFFER, instance->buffer);

	error = CheckALErrors("OALAudioEngine::CreateSoundFromBuffer alSourcei");

	if (error != AE_NO_ERROR)
	{
		alDeleteSources(1, &instance->source);
		alDeleteBuffers(1, &instance->buffer);
		delete instance;
		return error;
	}

	soundInstances.push_back(instance);

	*outSound = instance;

	return AE_NO_ERROR;
}
