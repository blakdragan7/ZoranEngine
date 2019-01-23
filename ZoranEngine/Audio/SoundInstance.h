#pragma once
#include <Audio/Common.h>
/******
Sound Instance

Opaque class the represents an instance of a loaded sound asset.
Mianly used to pass around sounds without the
user needing to know details about implementation

You must NEVER call delete on an instance of this class

******/
typedef unsigned AudioError;
class SoundInstance
{
public:
	AEImplementation type;
	SoundInstance() : type(AEI_INVALID) {}
	virtual ~SoundInstance() {}

	virtual AudioError SetGain(float gain) = 0;
	virtual AudioError GetGain(float& gain) = 0;
	virtual AudioError SetPitch(float pitch) = 0;
	virtual AudioError GetPitch(float& pitch) = 0;
	virtual AudioError SetMaxDistance(float max_distance) = 0;
	virtual AudioError GetMaxDistance(float& max_distance) = 0;
	virtual AudioError SetRolloff(float rolloff) = 0;
	virtual AudioError GetRolloff(float& rolloff) = 0;
	virtual AudioError SetReferenceDistance(float distance) = 0;
	virtual AudioError GetReferenceDistance(float& distance) = 0;
	virtual AudioError SetPosition(float x, float y, float z) = 0;
	virtual AudioError SetVelocity(float x, float y, float z) = 0;
	virtual AudioError SetDirection(float x, float y, float z) = 0;
	virtual AudioError SetIsRelative(bool isRelative) = 0;
	virtual AudioError GetIsRelative(bool& isRelative) = 0;
	virtual AudioError SetIsLooping(bool isLooping) = 0;
	virtual AudioError GetIsPlaying(bool& playing) = 0;
	virtual AudioError GetIsLooping(bool& looping) = 0;

};