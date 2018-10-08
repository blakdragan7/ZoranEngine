#pragma once
#include <unordered_map>
#include <Math/Vector2.h>


#include "ResourceBase.h"
/*
*	class used to represent a font asset that has been converted and is render to render
*/
class TextureBase;
class FontResourceInternal;

class ZoranEngine_EXPORT FontResource : public ResourceBase
{
private:
	TextureBase* fontTexture;
	std::unordered_map<uint32_t, Vector2D>* uvMap;

	FontResourceInternal* _data;

private:
	FontResource(); // only be created through Resource Manager

public:
	~FontResource();

	inline TextureBase* GetFontTexture()const { return fontTexture; }
	Vec2D UVForGlyph(uint32_t glyph);

	void CreateBMPForGlyphs(const std::vector<uint32_t>& glyphs);

	virtual int LoadFromFile(const std::string& file)override;
	virtual int SaveToFile(const std::string& file)override;
	virtual void DestroyResource()override;

	virtual const char* GetResourceDescription()const override;

	friend class ResourceManager;
};
