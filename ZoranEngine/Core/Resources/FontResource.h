#pragma once
#include <ThirdParty/sparsehash/dense_hash_map>
#include <Math/Vector2.h>
#include "ResourceBase.h"


static const char* ASCII = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()_-+=\\\"';:/?.>,<~` ";

/*
*	class used to represent a font asset that has been converted and is render to render
*/
class TextureBase;
class FontResourceInternal;

struct Glyph
{
	uint32_t glyph;
	Vector2D translate; // translating done to generation
	Vector2D UVOffset; // un UV Coords [0-1]
	Vector2D bearing; // in pixels
	Vector2D size; // in pixels
	Vector2D invScale; // in pixels
	Vector2D scale; // in pixels
	double advance; // in pixels
	float uvAdvance; // in UV Coords
	float scaleFactor;
};

enum FontSDFType
{
	Font_SDF_Type_MSDF = 0,
	Font_SDF_Type_PSDF = 1,
	Font_SDF_Type_SDF  = 2,
};

class ZoranEngine_EXPORT FontResource : public ResourceBase
{
private:
	TextureBase * fontTexture;
	google::dense_hash_map<uint32_t, Glyph>* glyphMap;

	FontResourceInternal* _data;
	std::string* sourcePath;
	std::string* zSourcePath;

	uint32_t	bmpResolution;
	float		pxRange;

	FontSDFType	type;

private:
	FontResource(uint32_t resolution, float pxRange, FontSDFType type); // only be created through Resource Manager
	FontResource(); // only be created through Resource Manager

	void GenerateFromSDF(const std::vector<uint32_t>& glyphs);
	void GenerateFromPSDF(const std::vector<uint32_t>& glyphs);
	void GenerateFromMSDF(const std::vector<uint32_t>& glyphs);

public:
	~FontResource();

	inline float GetPxRange() { return pxRange; }

	inline FontSDFType GetType() { return type; }

	inline TextureBase* GetFontTexture()const { return fontTexture; }
	const Glyph& GlyphForUnicode(uint32_t uni);

	inline size_t GetNumberOfGlyphs() { return glyphMap->size(); }
	
	void CreateBMPForGlyphs(const std::vector<uint32_t>& glyphs);
	void CreateBMPForASCII(const char* ascii);

	virtual int LoadFromFile(const std::string& file)override;
	virtual int SaveToFile(const std::string& file)override;
	virtual void DestroyResource()override;

	virtual const char* GetResourceDescription()const override;

	inline auto begin() { return glyphMap->begin(); }
	inline auto end() { return glyphMap->end(); }

	void NormalizeGlyphs();

	friend class ResourceManager;
};
