#pragma once
class ZoranEngine_EXPORT SoundAsset
{
private:
	bool isLoaded;

public:
	SoundAsset();
	~SoundAsset();

	int LoadFromFile(const std::string& file);
	int SaveToFile(const std::string& file);
	void DestroyAsset();

	const char* GetAssetDescription()const ;
};

