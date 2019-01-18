#pragma once
#include "Common.h"
#include <string>

/*****
AudioDevice

Opaque class that represent an AudioDevice. This can either be a hardware or software device.
the deviceDescription is essentially the name of the device and should be what the user sees when they
are choosing a device for playback.

You must NEVER call delete on an instance of this class

*****/
class AudioDevice
{
public:
	AEImplementation type;
	std::string deviceDescription;

	AudioDevice(std::string name) : deviceDescription(name), type(AEI_INVALID) {}
	virtual ~AudioDevice() {}
};