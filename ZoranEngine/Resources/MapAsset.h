#pragma once

// this asset represet the level loaded. So it includes all Objects in the map both instances and ZClasses

class ZClass;
class SceneObject;
class ZoranEngine_EXPORT MapAsset
{
private:
	// last file loaded from / saved to
	std::string* filePath;

	std::vector<SceneObject*>* levelObjects;

public:
	MapAsset();
	~MapAsset();

	int LoadFromFile(const std::string& file);
	int SaveToFile(const std::string& file);
	int Save();

	const std::string& GetFilePath()const { return *filePath; }
};

