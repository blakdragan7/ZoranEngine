#include "stdafx.h"
#include "OALSoundInstance.h"
#include "OALAudioEngine.h"

AudioError OALSoundInstance::SetGain(float gain)
{
	alSourcef(source, AL_GAIN, gain);
	return engine->CheckALErrors("SoundInstace::SetGain");
}

AudioError OALSoundInstance::GetGain(float& gain)
{
	alGetSourcef(source, AL_GAIN, &gain);
	return engine->CheckALErrors("SoundInstace::GetGain");
}

AudioError OALSoundInstance::SetPitch(float pitch)
{
	alSourcef(source, AL_PITCH, pitch);
	return engine->CheckALErrors("SoundInstace::SetPitch");
}

AudioError OALSoundInstance::GetPitch(float& pitch)
{
	alGetSourcef(source, AL_PITCH, &pitch);
	return engine->CheckALErrors("SoundInstace::GetPitch");
}

AudioError OALSoundInstance::SetMaxDistance(float max_distance)
{
	alSourcef(source, AL_MAX_DISTANCE, max_distance);
	return engine->CheckALErrors("SoundInstace::SetMaxDistance");
}

AudioError OALSoundInstance::GetMaxDistance(float& max_distance)
{
	alGetSourcef(source, AL_MAX_DISTANCE, &max_distance);
	return engine->CheckALErrors("SoundInstace::GetMaxDistance");
}

AudioError OALSoundInstance::SetRolloff(float rolloff)
{
	alSourcef(source, AL_ROLLOFF_FACTOR, rolloff);
	return engine->CheckALErrors("SoundInstace::SetRolloff");
}

AudioError OALSoundInstance::GetRolloff(float& rolloff)
{
	alGetSourcef(source, AL_ROLLOFF_FACTOR, &rolloff);
	return engine->CheckALErrors("SoundInstace::GetRolloff");
}

AudioError OALSoundInstance::SetReferenceDistance(float distance)
{
	alSourcef(source, AL_REFERENCE_DISTANCE, distance);
	return engine->CheckALErrors("SoundInstace::SetReferenceDistance");
}

AudioError OALSoundInstance::GetReferenceDistance(float& distance)
{
	alGetSourcef(source, AL_REFERENCE_DISTANCE, &distance);
	return engine->CheckALErrors("SoundInstace::GetReferenceDistance");
}

AudioError OALSoundInstance::SetPosition(float x, float y, float z)
{
	alSource3f(source,AL_POSITION,x,y,z);
	return engine->CheckALErrors("SoundInstace::SetPosition");
}

AudioError OALSoundInstance::SetVelocity(float x, float y, float z)
{
	alSource3f(source, AL_VELOCITY, x, y, z);
	return engine->CheckALErrors("SoundInstace::SetVelocity");
}

AudioError OALSoundInstance::SetDirection(float x, float y, float z)
{
	alSource3f(source, AL_DIRECTION, x, y, z);
	return engine->CheckALErrors("SoundInstace::SetDirection");
}

AudioError OALSoundInstance::SetIsRelative(bool isRelative)
{
	ALint uRelative = isRelative ? AL_TRUE : AL_FALSE;
	alSourcei(source, AL_SOURCE_RELATIVE, uRelative);
	return engine->CheckALErrors("SoundInstace::SetIsRelative");
}

AudioError OALSoundInstance::GetIsRelative(bool& isRelative)
{
	ALint uRelative = 0;
	alGetSourcei(source, AL_SOURCE_RELATIVE, &uRelative);
	isRelative = uRelative == AL_TRUE;
	return engine->CheckALErrors("SoundInstace::GetIsRelative");
}

AudioError OALSoundInstance::SetIsLooping(bool isLooping)
{
	ALint uRelative = isLooping ? AL_TRUE : AL_FALSE;
	alSourcei(source, AL_LOOPING, uRelative);
	return engine->CheckALErrors("SoundInstace::SetIsLooping");
}

AudioError OALSoundInstance::GetIsPlaying(bool& playing)
{
	ALint uRelative = 0;
	alGetSourcei(source, AL_SOURCE_STATE, &uRelative);
	playing = uRelative == AL_PLAYING;
	return engine->CheckALErrors("SoundInstace::GetIsPlaying");
}

AudioError OALSoundInstance::GetIsLooping(bool& looping)
{
	ALint uRelative = 0;
	alGetSourcei(source, AL_LOOPING, &uRelative);
	looping = uRelative == AL_TRUE;
	return engine->CheckALErrors("SoundInstace::GetIsLooping");
}