#pragma once

#include <Core/Audio/AudioEngineBase.h>

class OALAudioDevice;
class OALAudioListener;
class OALAudioEngine : public AudioEngineBase
{
private:
	OALAudioDevice * currentDevice;
	OALAudioListener * activeListener;

	AudioError ALCenumToALError(int);
	AudioError CheckErrors(const char* func);

public:
	OALAudioEngine();
	~OALAudioEngine();

	virtual AudioError Init(AudioDevice * toDevice)override;
	virtual AudioError SwitchToDevice(AudioDevice* toDevice, AudioDevice* fromDevice)override;
	virtual AudioError  CreateAudioListener(const SceneObject3D * object, AudioListener** outListener)override;
	virtual AudioError  CreateAudioListener(const SceneObject2D * object, AudioListener** outListener)override;
	virtual AudioError MakeListenerActive(AudioListener* listener)override;
	virtual AudioError DestroyListener(AudioListener* listener)override;
	virtual AudioError  PlaySoundAtLocation(SoundInstance* sound, const Vector3D& location)override;
	virtual AudioError  PlaySoundAtLocation(SoundInstance* sound, const Vector2D& location)override;
	virtual AudioError  PlaySoundInstance(SoundInstance* sound)override;
	virtual void  StopSound(SoundInstance* sound)override;
	virtual bool  QuerryCapability(AudioCapability cap)override;
	virtual AudioError  SetCapability(AudioCapability cap)override;
	virtual AudioError  SetCapability(std::initializer_list<AudioCapability> list)override;
	virtual AudioError CreateSoundFromFile(const char* file, AudioFileType type, SoundInstance** outSound)override;
	virtual AudioError CreateSoundFromBuffer(void* buffer, AudioBufferType bType, SoundInstance** outSound)override;
	virtual AudioError DestroySound(SoundInstance* outSound)override;
	virtual const char* StringForError(AudioError error)override;
};

