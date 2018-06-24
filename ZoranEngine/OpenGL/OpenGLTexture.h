#pragma once
#include "Rendering/TextureBase.h"

// as an opengl class this must be created on the draw thread, usually the main thread

class ZoranEngine_EXPORT OpenGLTexture : public TextureBase
{
private:
	unsigned gl_texture;
	static bool glTextureIsEnabled;
private:
	unsigned GLTypeFromRenderDataType(RenderDataType type);
	unsigned GLFormatFromRenderDataFormat(RenderDataFormat format);

public:
	OpenGLTexture(RenderEngineBase* engine, RenderDataType type = TYPE_RGBA_32, RenderDataFormat format = FORMAT_UNSIGNED_BYTE);
	~OpenGLTexture();
	// these may get moved to TextureBase it depends on how DirectX handles textures
	void BindTexture(unsigned textureNumber);
	void UnbindTexture(unsigned textureNumber);

	virtual void LoadFromPath(const char* texture_path, RenderDataType type = TYPE_BGRA_32, RenderDataFormat format = FORMAT_UNSIGNED_BYTE)override;
	virtual void LoadFromMemory(unsigned x, unsigned y, void* data, RenderDataType type = TYPE_BGRA_32, RenderDataFormat format = FORMAT_UNSIGNED_BYTE)override;
	virtual void SetRenderDataType(RenderDataType newType)override;
	virtual void SetRenderDataFormat(RenderDataFormat newFormat)override;

	virtual void UseTexture(void* data);
	virtual void StopUsingTexture(void* data);
};
