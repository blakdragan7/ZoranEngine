#pragma once
#include <initializer_list>
class Vector2D;
class Vector3D;
class SoundInstance;
class AudioListener;
typedef unsigned AudioError;
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
public:
	AudioEngineBase() {}
	virtual ~AudioEngineBase() {}

	/* initialize sound engine and connect to default sound device */
	virtual void Init() = 0;
	/* Creats an audio listener at location */
	virtual AudioError  CreateAudioListener(const Vector3D& location, AudioListener* outListener) = 0;
	/* Same as above but with z = 0 */
	virtual AudioError  CreateAudioListener(const Vector2D& location, AudioListener* outListener) = 0;
	/* Used to teardown audio listener and free all memory assosiated with it. */
	virtual AudioError DestroyListener(AudioListener* listener) = 0;
	/* Sets the currently active listener, using 0 means all listeners */
	virtual AudioError  SetListener(AudioListener* listener) = 0;
	/*  Plays a sound with 3D audio at a location. The current listener then receives audio in a >= 2.1 channel way if available*/
	virtual AudioError  PlaySoundAtLocation(const Vector3D& location) = 0;
	/* Same as above but always places z as the same as the listener */
	virtual AudioError  PlaySoundAtLocation(const Vector2D& location) = 0;
	/* Play non 3d sound, like background music */
	virtual AudioError  PlaySound() = 0;
	/* stops a specific sound during play, if the sound isn't playing it just does nothing */
	virtual void  StopSound() = 0;
	/* Asks the egnine if it supports a specific capability, like doppler effect */
	virtual bool  QuerryCapability(enum Capability cap) = 0;
	/* Turns a specific capability on. returns AE_NOT_SUPPORTED if not possible */
	virtual AudioError  SetCapability(enum Capability cap) = 0;
	/* Mass Select capabilites, if any of the capabilities are not supported the return result will be AE_NOT_SUPPORTED and none will be set*/
	virtual AudioError  SetCapability(std::initializer_list<enum Capability> list) = 0;
	/* Creates a sound and stores it in the audio engine with givent type. if the type is not supported, AE_NOT_SUPPORTED is returned. */
	virtual AudioError CreateSoundFromFile(const char* file,enum AudioType type, SoundInstance* outSound) = 0;
	/* Same as above but from a buffer containing the audio instead */
	virtual AudioError CreateSoundFromBuffer(void* buffer,enum AudioBufferType bType,enum AudioType aType, SoundInstance* outSound) = 0;
	/* Destory instance of sound and frees memory assosiated with that sound, returns AE_INVALID_OPERATION if sound is not valid */
	virtual AudioError DestroySound(SoundInstance* outSound) = 0;
	/* Gives Human Readable string for error or 0 if not a valid error */
	virtual const char* StringForError(AudioError error) = 0;
};

/******
Audio Listener

Opaque class the represents the AudioListener.
Mianly used to pass around instance of audio listener without the
user needing to know details about implementation

******/
class AudioListener
{

};

/******
Sound Instance

Opaque class the represents an instance of a loaded sound asset.
Mianly used to pass around sounds without the
user needing to know details about implementation

******/
class SoundInstance
{

};