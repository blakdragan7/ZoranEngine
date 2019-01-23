#pragma once
#include <Audio/Common.h>

/******
Audio Listener

Opaque class the represents the AudioListener.
Mianly used to pass around instance of audio listener without the
user needing to know details about implementation

You must NEVER call delete on an instance of this class

******/
typedef unsigned AudioError;
class AudioListener
{
public:
	AEImplementation type;
	AudioListener() : type(AEI_INVALID) {}
	virtual ~AudioListener() {}

	virtual AudioError SetGain(float gain) = 0;
	virtual AudioError GetGain(float& gain) = 0;
	virtual AudioError SetPosisition(float x, float y, float z) = 0;
	virtual AudioError SetVelocity(float x, float y, float z) = 0;
	virtual AudioError SetOrientation(float upx, float upy, float upz, float forwardx, float forwardy, float forwardz) = 0;
};