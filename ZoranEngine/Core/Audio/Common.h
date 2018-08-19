#pragma once

#define AE_NO_ERROR 0

/* Usually returned when some operation was attempted that isn't supported. 
such as turning on a capability that this engine implementation doesnt supprot*/
#define AE_NOT_SUPPORTED 1
/* Returned when In invalid state (like playing audio with no listeners) */
#define AE_INVALID_OPERATION 2
/* Returned if an audio function is called without calling Init first */
#define AE_NOT_INITIALIZED 3
/* Returned if an audio device is passed to an engine when that audio device was not created by that engine */
#define AE_INVALID_AUDIO_DEVICE 4
/* Returned if a sound instance is passed to an engine when that sound instance was not created by that engine */
#define AE_INVALID_SOUND_INSTANCE 5
/* Returned if an audio listener is passed to an engine when that audio listener was not created by that engine */
#define AE_INVALID_AUDIO_LISTENER 6

enum AudioFileType
{
	AFT_MP3,
	AFT_WAV
};

enum AudioBufferType
{
	ABT_LPCM8,
	ABT_LPCM16,
	ABT_LPCM32,
};

enum AudioCapability
{
	AC_DOPPLER,
	AC_REVERB,
	AC_LOW_PASS,
	AC_HIGH_PASS
};

enum AEImplementation
{
	AEI_INVALID,
	AEI_OAL
};

/******
Audio Listener

Opaque class the represents the AudioListener.
Mianly used to pass around instance of audio listener without the
user needing to know details about implementation

You must NEVER call delete on an instance of this class

******/
class AudioListener
{
public:
	AEImplementation type;
	AudioListener() : type(AEI_INVALID) {}
	virtual ~AudioListener() {}
};

/******
Sound Instance

Opaque class the represents an instance of a loaded sound asset.
Mianly used to pass around sounds without the
user needing to know details about implementation

You must NEVER call delete on an instance of this class

******/
class SoundInstance
{
public:
	AEImplementation type;
	SoundInstance() : type(AEI_INVALID) {}
	virtual ~SoundInstance() {}
};
/*****
AudioDevice

Opaque class that represent an AudioDevice. This can either be a hardware or software device.
the deviceDescription is essentially the name of the device and should be what the user sees when they
are choosing a device for playback.

*****/
class AudioDevice
{
public:
	AEImplementation type;
	std::string deviceDescription;

	AudioDevice(std::string name) : deviceDescription(name), type(AEI_INVALID) {}
	virtual ~AudioDevice() {}
};