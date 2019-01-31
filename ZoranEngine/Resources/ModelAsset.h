#pragma once
class ModelRenderer;
class Matrix44;
class ZoranEngine_EXPORT ModelAsset
{
private:
	bool isLoaded;
	ModelRenderer* renderAsset;

	std::string* sourcePath;
	std::string* zSourcePath;

public:
	ModelAsset();
	ModelAsset(ModelRenderer* renderAsset);
	~ModelAsset();

	int MakeFromFile(const std::string& file);
	int LoadFromFile(const std::string& file);
	int SaveToFile(const std::string& file);
	int Save();

	const char* GetAssetDescription()const;

	void RenderObject(const Matrix44& matrix);
};

