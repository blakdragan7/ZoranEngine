#include "stdafx.h"
#include "OALAudioListener.h"
#include "OALAudioEngine.h"
#include <ThirdParty/AL/al.h>

AudioError OALAudioListener::SetGain(float gain)
{
	if (isListening == false)alcMakeContextCurrent(context);
	alListenerf(AL_GAIN, gain);
	if (isListening == false)engine->MakeActiveListenerActive();
	return engine->CheckALErrors("AudioListener::SetGain");
}

AudioError OALAudioListener::GetGain(float& gain)
{
	if (isListening == false)alcMakeContextCurrent(context);
	alGetListenerf(AL_GAIN, &gain);
	if (isListening == false)engine->MakeActiveListenerActive();
	return engine->CheckALErrors("AudioListener::GetGain");
}

AudioError OALAudioListener::SetPosisition(float x, float y, float z)
{
	if (isListening == false)alcMakeContextCurrent(context);
	alListener3f(AL_POSITION, x,y,z);
	if (isListening == false)engine->MakeActiveListenerActive();
	return engine->CheckALErrors("AudioListener::SetPosisition");
}

AudioError OALAudioListener::SetVelocity(float x, float y, float z)
{
	if (isListening == false)alcMakeContextCurrent(context);
	alListener3f(AL_VELOCITY, x, y, z);
	if (isListening == false)engine->MakeActiveListenerActive();
	return engine->CheckALErrors("AudioListener::SetVelocity");
}

AudioError OALAudioListener::SetOrientation(float upx, float upy, float upz, float forwardx, float forwardy, float forwardz)
{
	float values[6] = { upx,upy,upz,forwardx,forwardy,forwardz };

	if (isListening == false)alcMakeContextCurrent(context);
	alListenerfv(AL_ORIENTATION, values);
	if (isListening == false)engine->MakeActiveListenerActive();
	return engine->CheckALErrors("AudioListener::SetOrientation");
}
