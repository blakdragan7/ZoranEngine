#pragma once

#include <Interfaces/ITickableObject.h>

#include <Core/Audio/Common.h>

class AudioListener;
class ZoranEngine_EXPORT SpeakerComponentBase : public ITickableObject
{
private:
	AudioListener * internalListener;

public:
	SpeakerComponentBase();
	virtual ~SpeakerComponentBase();

	/* audio listener passthrough */

	AudioError SetGain(float gain);
	AudioError GetGain(float& gain);
	AudioError SetPosisition(float x, float y, float z = 0);
	AudioError SetVelocity(float x, float y, float z = 0);
	AudioError SetOrientation(float upx, float upy, float upz, float forwardx, float forwardy, float forwardz);
};

