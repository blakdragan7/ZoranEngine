#pragma once

class SoundInstance;
class AudioListener;
class ZoranEngine_EXPORT SoundAsset
{
private:
	bool isLoaded;
	SoundInstance* playableAsset;
	AudioListener* listener;

public:
	SoundAsset();
	SoundAsset(AudioListener* listener);
	~SoundAsset();

	void AssignListener(AudioListener* listener);

	void Play();
	void PlayAtLocation(Vec3D location);
	void PlayAtLocation(Vec2D location);

	void Stop();

	int MakeFromFile(const std::string& file);
	int LoadFromFile(const std::string& file);
	int SaveToFile(const std::string& file);

	const char* GetAssetDescription()const ;
};

