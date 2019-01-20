#pragma once
#include <Rendering/RenderTypes.h>
#include <Rendering/TextureBase.h>

class ZoranEngine_EXPORT ImageAsset
{
private:
	TextureBase* renderAsset;

	bool isLoaded;

public:
	ImageAsset();
	ImageAsset(TextureBase* renderAsset);
	~ImageAsset();

	inline TextureBase* GetRenderAsset()const { return renderAsset; }

	int LoadFromFile(const std::string& file, RenderDataType type, RenderDataFormat format);
	int LoadFromFile(const std::string& file);
	int SaveToFile(const std::string& file);
	void DestroyAsset();

	const char* GetAssetDescription()const;

	// TextureBase Accessors

	inline RenderDataType GetRenderDataType()const { return renderAsset->GetRenderDataType(); }
	inline RenderDataFormat GetRenderDataFormat()const { return  renderAsset->GetRenderDataFormat(); }

	bool GetContainsData()const { return renderAsset->GetContainsData(); }

	/* data argument is available in case d3d needs something to be passed to use OpenGL Textures just use this argument as an int*/
	virtual void UseTexture(unsigned long data)const { renderAsset->UseTexture(data); };
	virtual void StopUsingTexture(unsigned long  data)const { renderAsset->StopUsingTexture(data); };

	/* returns the render engine specific id */
	virtual unsigned GetTextureID()const { return renderAsset->GetTextureID(); };

	inline Vector2I GetSize()const { return renderAsset->GetSize(); };
};

