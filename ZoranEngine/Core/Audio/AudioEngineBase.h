#pragma once
#include <initializer_list>
#include <list>

class Vector2D;
class Vector3D;
class SoundInstance;
class AudioListener;
class AudioDevice;
class SceneObject3D;
class SceneObject2D;
typedef unsigned AudioError;
enum AudioBufferType;
enum AudioFileType;
enum AudioCapability;
/*******

Audio Engine Base

This abstract class serves as an interface for playing 3D audio
It is up to the sub class to implement this in someway and to make it cross platform. 

the number of channels supported is up to the implementation

The Audio Engine is a giant state machine, event though the implementation may be different, each impementation works in this order,
first, set the state of the audio (ex. turn on echo and attenuation). then play the sound. 

This is modeled this way becuase OpenAL works this way and OpenAL will be the first implementation of the audio engine.
This means that it is much more efficient to play all sound with the same settings at once though this is ususally not possible.
Because of that it is MUCH more efficient to bake the sound effects into the audio where possible so there is minimal setting changes between sounds

********/
class AudioEngineBase
{
protected: /* List of every device,listenr and sound created by the engine */
	std::list<AudioDevice*> audioDevices;
	std::list<AudioListener*> audioListeners;
	std::list<SoundInstance*> soundInstances;

public:
	AudioEngineBase() {}
	virtual ~AudioEngineBase();

	/* initialize sound engine and connects to device. If device is 0, default audio device is used */
	virtual AudioError Init(AudioDevice* device) = 0;
	/* Switch to device destroying and re creating all objects assosiated with that device internally. 
	The previously created listener and sound isntances will still be valid. this operation will be pretty 
	slow so it should either be done in a background thread or warn the user it is happening. */
	virtual AudioError SwitchToDevice(AudioDevice* toDevice, AudioDevice* fromDevice) = 0;
	/* Creats an audio listener with location, orientation and veleocity of object, if there is no active listener then sets this listener active */
	virtual AudioError  CreateAudioListener(const SceneObject3D * object, AudioListener** outListener) = 0;
	/* Same as above but with a 2d scene object */
	virtual AudioError  CreateAudioListener(const SceneObject2D * object, AudioListener** outListener) = 0;
	/* makes the specific listener the one that receives sounds */
	virtual AudioError MakeListenerActive(AudioListener* listener) = 0;
	/* Used to teardown audio listener and free all memory assosiated with it. */
	virtual AudioError DestroyListener(AudioListener* listener) = 0;
	/*  Plays a sound with 3D audio at a location. The current listener then receives audio in a >= 2.1 channel way if available*/
	virtual AudioError  PlaySoundAtLocation(SoundInstance* sound, const Vector3D& location) = 0;
	/* Same as above but always places z as the same as the listener */
	virtual AudioError  PlaySoundAtLocation(SoundInstance* sound,const Vector2D& location) = 0;
	/* Play non 3d sound, like background music */
	virtual AudioError  PlaySoundInstance(SoundInstance* sound) = 0;
	/* stops a specific sound during play, if the sound isn't playing it just does nothing */
	virtual void  StopSound(SoundInstance* sound) = 0;
	/* Asks the egnine if it supports a specific capability, like doppler effect */
	virtual bool  QuerryCapability(AudioCapability cap) = 0;
	/* Turns a specific capability on. returns AE_NOT_SUPPORTED if not possible */
	virtual AudioError  SetCapability(AudioCapability cap) = 0;
	/* Mass Select capabilites, if any of the capabilities are not supported the return result will be AE_NOT_SUPPORTED and none will be set*/
	virtual AudioError  SetCapability(std::initializer_list<AudioCapability> list) = 0;
	/* Creates a sound and stores it in the audio engine with givent type. if the type is not supported, AE_NOT_SUPPORTED is returned. */
	virtual AudioError CreateSoundFromFile(const char* file,AudioFileType type, SoundInstance** outSound) = 0;
	/* Same as above but from a buffer containing the audio instead */
	virtual AudioError CreateSoundFromBuffer(void* buffer,AudioBufferType bType, SoundInstance** outSound) = 0;
	/* Destory instance of sound and frees memory assosiated with that sound, returns AE_INVALID_OPERATION if sound is not valid */
	virtual AudioError DestroySound(SoundInstance* sound) = 0;
	/* Gives Human Readable string for error or 0 if not a valid error */
	virtual const char* StringForError(AudioError error);
	/* Returns all possible audio devices available for playback. */
	inline const std::list<AudioDevice*>& GetPossibleDevices() { return audioDevices; }
};

