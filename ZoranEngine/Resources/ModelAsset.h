#pragma once
class ZoranEngine_EXPORT ModelAsset
{
private:
	bool isLoaded;

public:
	ModelAsset();
	~ModelAsset();

	int LoadFromFile(const std::string& file);
	int SaveToFile(const std::string& file);
	void DestroyAsset();

	const char* GetAssetDescription()const;
};

