#pragma once
#include <ThirdParty/sparsehash/dense_hash_map>
#include <Math/Vector2.h>

#include <Resources/ImageResource.h>

static const char* ASCII = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()_-+=\\\"';:/?.>,<~` ";

/*
*	class used to represent a font asset that has been converted and is render to render
*/
class FontAssetInternal;

struct Glyph
{
	uint32_t glyph;
	Vector2D translate; // translating done to generation
	Vector2D UVOffset; // un UV Coords [0-1]
	Vector2D bearing; // in normalized screen coords
	Vector2D size; // in normalized screen coords
	Vector2D invScale; // in normalized screen coords
	Vector2D scale; // in normalized screen coords
	float advance; // in normalized screen coords
	float uvAdvance; // in UV Coords
	float scaleFactor;
};

enum FontSDFType
{
	Font_SDF_Type_MSDF = 0,
	Font_SDF_Type_PSDF = 1,
	Font_SDF_Type_SDF  = 2,
};

class ZoranEngine_EXPORT FontAsset
{
private:
	ImageResource fontTexture;
	google::dense_hash_map<uint32_t, Glyph>* glyphMap;

	FontAssetInternal* _data;
	std::string* sourcePath;
	std::string* zSourcePath;

	uint32_t	bmpResolution;
	float		pxRange;

	FontSDFType	type;

	bool isLoaded;

private:
	void GenerateFromSDF(const std::vector<uint32_t>& glyphs);
	void GenerateFromPSDF(const std::vector<uint32_t>& glyphs);
	void GenerateFromMSDF(const std::vector<uint32_t>& glyphs);

public:
	FontAsset(uint32_t resolution, float pxRange, FontSDFType type);
	FontAsset();
	~FontAsset();

	inline float GetPxRange() { return pxRange; }

	inline FontSDFType GetType() { return type; }

	inline ImageResource GetFontTexture()const { return fontTexture; }
	const Glyph& GlyphForUnicode(uint32_t uni);

	inline size_t GetNumberOfGlyphs() { return glyphMap->size(); }
	
	void CreateBMPForGlyphs(const std::vector<uint32_t>& glyphs);
	void CreateBMPForASCII(const char* ascii);

	int MakeFromFile(const std::string& file);
	int LoadFromFile(const std::string& file);
	int SaveToFile(const std::string& file);
	int Save();

	const char* GetResourceDescription()const;

	inline auto begin() { return glyphMap->begin(); }
	inline auto end() { return glyphMap->end(); }

	void NormalizeGlyphs();

	friend class ResourceManager;
};
