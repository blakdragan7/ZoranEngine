#include "stdafx.h"
#include "SpeakerComponentBase.h"

#include <Audio/AudioEngineBase.h>
#include <Audio/AudioListener.h>

SpeakerComponentBase::SpeakerComponentBase()
{
	AudioError err = aEngine->CreateAudioListener(&internalListener);
	if (err != AE_NO_ERROR)
	{
		Log(LogLevel_Error,"Could Not Create Audio Listener For Component !!\nError: %s",aEngine->StringForError(err));
	}
	zEngine->AddTickableObject(this);
}


SpeakerComponentBase::~SpeakerComponentBase()
{
	zEngine->RemoveTickableObject(this);
	aEngine->DestroyListener(internalListener);
}

AudioError SpeakerComponentBase::SetGain(float gain)
{
	return internalListener->SetGain(gain);
}

AudioError SpeakerComponentBase::GetGain(float & gain)
{
	return internalListener->GetGain(gain);
}

AudioError SpeakerComponentBase::SetPosisition(float x, float y, float z)
{
	return internalListener->SetPosisition(x, y, z);
}

AudioError SpeakerComponentBase::SetVelocity(float x, float y, float z)
{
	return internalListener->SetVelocity(x, y, z);
}

AudioError SpeakerComponentBase::SetOrientation(float upx, float upy, float upz, float forwardx, float forwardy, float forwardz)
{
	return internalListener->SetOrientation(upx, upy, upz, forwardx, forwardy, forwardz);
}
