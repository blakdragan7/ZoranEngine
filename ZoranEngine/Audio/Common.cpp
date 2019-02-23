#include "stdafx.h"
#include "Common.h"

const char * StringForAudioError(AudioError error)
{
	switch (error)
	{
	case AE_NO_ERROR:
		return "No Audio Error";
	case AE_NOT_SUPPORTED:
		return "Not Supported Audio Error";
	case AE_INVALID_OPERATION:
		return "Invalid Audio Operation";
	case AE_NOT_INITIALIZED:
		return "Audio Engine Not Initialized";
	case AE_INVALID_AUDIO_DEVICE:
		return "Invalid Audio Device";
	case AE_INVALID_SOUND_INSTANCE:
		return "Invalid Sound Instance";
	case AE_INVALID_AUDIO_LISTENER:
		return "Invalid Audio Listener";
	case AE_NOT_FOUND:
		return "Audio Error Not Found";
	case AE_UNKOWN_ERROR:
		return "Unkown Audio Error";
	case AE_NO_MEMORY:
		return "Audio Out Of Memeory";
	case AE_BACKEND_ERROR:
		return "Audio Backend Error";
	case AE_NO_DEVICE:
		return "No Audio Device";
	}

	return "Unsuported Audo Error Enumn";
}
