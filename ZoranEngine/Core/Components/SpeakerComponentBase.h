#pragma once

#include <Interfaces/ITickableObject.h>

#include <Audio/Common.h>

#include <Resources/ResourceTypes.h>

class AudioListener;
class ZoranEngine_EXPORT SpeakerComponentBase : public ITickableObject
{
private:
	AudioListener * internalListener;
	SoundResource sound;

public:
	SpeakerComponentBase();
	virtual ~SpeakerComponentBase();

	/* audio listener passthrough */

	AudioError SetGain(float gain);
	AudioError GetGain(float& gain);
	AudioError SetPosisition(float x, float y, float z = 0);
	AudioError SetVelocity(float x, float y, float z = 0);
	AudioError SetOrientation(float upx, float upy, float upz, float forwardx, float forwardy, float forwardz);

	inline void SetSound(SoundResource newSound) { sound = newSound; }
	inline SoundResource GetSound() { return sound; }

	void PlayAudio();
	void StopAudio();
};

