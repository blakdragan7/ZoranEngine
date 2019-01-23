#pragma once
class ZoranEngine_EXPORT ModelAsset
{
private:
	bool isLoaded;

public:
	ModelAsset();
	~ModelAsset();

	int MakeFromFile(const std::string& file);
	int LoadFromFile(const std::string& file);
	int SaveToFile(const std::string& file);

	const char* GetAssetDescription()const;
};

