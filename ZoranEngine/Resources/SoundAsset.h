#pragma once

class SoundInstance;
class AudioListener;
class ZoranEngine_EXPORT SoundAsset
{
private:
	bool isLoaded;
	SoundInstance* playableAsset;
	AudioListener* listener;
	std::string* sourceFile;
	std::string* zSourcePath;

public:
	SoundAsset();
	SoundAsset(AudioListener* listener);
	~SoundAsset();

	void AssignListener(AudioListener* listener);

	void Play();
	void PlayAtLocation(Vec3D location);
	void PlayAtLocation(Vec2D location);
	
	void Stop();

	// listener overloads
	void Play(AudioListener* listener);
	void PlayAtLocation(Vec3D location, AudioListener* listener);
	void PlayAtLocation(Vec2D location, AudioListener* listener);

	void Stop(AudioListener* listener);

	int MakeFromFile(const std::string& file, AudioListener* listener = 0);
	int LoadFromFile(const std::string& file, AudioListener* listener = 0);
	int SaveToFile(const std::string& file);

	const char* GetAssetDescription()const ;
};

