#pragma once
#include <Audio/AudioListener.h>
#include <ThirdParty/AL/alc.h>
class OALAudioEngine;
class OALAudioListener : public AudioListener
{
private:
	ALCcontext * context;
	bool isListening;
	OALAudioEngine* engine;

	OALAudioListener(OALAudioEngine* engine) : AudioListener(), engine(engine), isListening(false), context(0) { type = AEI_OAL; }
	~OALAudioListener() {}

	friend OALAudioEngine;
public:

	virtual AudioError SetGain(float gain)override;
	virtual AudioError GetGain(float &gain)override;
	virtual AudioError SetPosisition(float x, float y, float z)override;
	virtual AudioError SetVelocity(float x, float y, float z)override;
	virtual AudioError SetOrientation(float upx, float upy, float upz, float forwardx, float forwardy, float forwardz)override;
};