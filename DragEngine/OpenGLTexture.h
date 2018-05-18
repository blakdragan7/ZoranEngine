#pragma once
#include "TextureBase.h"

// as an opengl class this must be created on the draw thread, usually the main thread

class DRAGENGINE_EXPORT OpenGLTexture : public TextureBase
{
private:
	unsigned gl_texture;

public:
	OpenGLTexture(RenderEngine* engine, RenderDataType type = TYPE_BGRA_32, RenderDataFormat format = FORMAT_UNSIGNED_BYTE);
	~OpenGLTexture();

	virtual void LoadFromPath(const char* texture_path, RenderDataType type = TYPE_BGRA_32, RenderDataFormat format = FORMAT_UNSIGNED_BYTE)override;
	virtual void LoadFromMemory(unsigned x, unsigned y, void* data, RenderDataType type = TYPE_BGRA_32, RenderDataFormat format = FORMAT_UNSIGNED_BYTE)override;
	virtual void SetRenderDataType(RenderDataType newType)override;
	virtual void SetRenderDataFormat(RenderDataFormat newFormat)override;
};

