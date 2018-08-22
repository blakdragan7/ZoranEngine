#pragma once
#include <Core/Audio/Common.h>
#include <ThirdParty/AL/al.h>
#include <ThirdParty/AL/alc.h>

class OALAudioDevice : public AudioDevice
{
private:
	ALCdevice* oalDevice;

	OALAudioDevice(std::string name) : AudioDevice(name), oalDevice(0) { type = AEI_OAL; }
	~OALAudioDevice() {}

	friend class OALAudioEngine;
};

class OALAudioListener : public AudioListener
{
private:
	ALCcontext * context;
	bool isListening;

	OALAudioListener() : AudioListener(), isListening(false), context(0) { type = AEI_OAL; }
	~OALAudioListener() {}

	friend class OALAudioEngine;
};

class OALSoundInstace : public SoundInstance 
{
public:
	OALSoundInstace() { type = AEI_OAL; }
	~OALSoundInstace() {}

	friend class OALAudioEngine;
};
