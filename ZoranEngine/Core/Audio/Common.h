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
/* Returned if some part of the function was not found, for example unable to find default audio device */
#define AE_NOT_FOUND 7
/* ususally only returned if the underlying implementation, like openal returns unkown error */
#define AE_UNKOWN_ERROR 8
/* The engine ran out of memory */
#define AE_NO_MEMORY 9
/* there was an error with the backend implementation, probably a bug */
#define AE_BACKEND_ERROR 10

enum AudioFileType
{
	AFT_MP3,
	AFT_WAV
};

enum AudioBufferType
{
	ABT_LPCM_MONO_8,
	ABT_LPCM_MONO_16,
	ABT_LPCM_STEREO_8,
	ABT_LPCM_STEREO_16,
	
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
