#pragma once
#include <Core/Audio/SoundInstance.h>
#include <ThirdParty/AL/al.h>

class OALAudioEngine;
class OALSoundInstance : public SoundInstance
{
private:
	ALuint buffer, source;
	unsigned bufferSize;
	OALAudioEngine* engine;

	OALSoundInstance(OALAudioEngine* engine) : engine(engine) ,bufferSize(0), buffer(0), source(0) { type = AEI_OAL; }
	~OALSoundInstance() {}

	friend OALAudioEngine;
public:

	virtual AudioError SetGain(float gain)override;
	virtual AudioError GetGain(float& gain)override;
	virtual AudioError SetPitch(float pitch)override;
	virtual AudioError GetPitch(float& pitch)override;
	virtual AudioError SetMaxDistance(float max_distance)override;
	virtual AudioError GetMaxDistance(float& max_distance)override;
	virtual AudioError SetRolloff(float rolloff)override;
	virtual AudioError GetRolloff(float& rolloff)override;
	virtual AudioError SetReferenceDistance(float distance)override;
	virtual AudioError GetReferenceDistance(float& distance)override;
	virtual AudioError SetPosition(float x, float y, float z)override;
	virtual AudioError SetVelocity(float x, float y, float z)override;
	virtual AudioError SetDirection(float x, float y, float z)override;
	virtual AudioError SetIsRelative(bool isRelative)override;
	virtual AudioError GetIsRelative(bool& isRelative)override;
	virtual AudioError SetIsLooping(bool isLooping)override;
	virtual AudioError GetIsPlaying(bool& playing)override;
	virtual AudioError GetIsLooping(bool& looping)override;

};