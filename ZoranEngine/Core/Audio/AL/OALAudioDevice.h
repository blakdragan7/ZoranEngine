#pragma once
#include <Core/Audio/AudioDevice.h>
#include <ThirdParty/AL/alc.h>

class OALAudioDevice : public AudioDevice
{
private:
	ALCdevice * oalDevice;

	OALAudioDevice(std::string name) : AudioDevice(name), oalDevice(0) { type = AEI_OAL; }
	~OALAudioDevice() {}

	friend class OALAudioEngine;
};