#include "stdafx.h"
#include "AudioEngineBase.h"
#include "Common.h"

AudioEngineBase::~AudioEngineBase()
{
	
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
	case AE_INVALID_AUDIO_DEVICE:
		return "Invalid Audio Device";
		break;
	case AE_INVALID_SOUND_INSTANCE:
		return "Invalid Sound Instance";
		break;
	case AE_INVALID_AUDIO_LISTENER:
		return "Invalid Audio Listener";
		break;
	case AE_NOT_FOUND:
		return "Not Found";
		break;
	case AE_UNKOWN_ERROR:
		return "Unkown Error";
		break;
	case AE_NO_MEMORY:
		return "Out Of Memory";
		break;
	case AE_BACKEND_ERROR:
		return "Backend Error";
		break;
	case AE_NO_DEVICE:
		return "No Device";
		break;
	default:
		return 0;
	}
}
