#pragma once
#include "Rendering/TextureBase.h"

// as an opengl class this must be created on the draw thread, usually the main thread
class OpenGLContext;
class OpenGLTexture : public TextureBase
{
private:
	unsigned gl_texture;
	OpenGLContext* OGL;
	
	static bool glTextureIsEnabled;

private:
	unsigned GLTypeFromRenderDataType(RenderDataType type);
	unsigned GLFormatFromRenderDataFormat(RenderDataFormat format);

public:
	OpenGLTexture(OpenGLContext* OGL, RenderDataType type = Render_Data_Type_BGRA_32, RenderDataFormat format = Render_Data_Format_Unsigned_Byte);
	~OpenGLTexture();
	// these may get moved to TextureBase it depends on how DirectX handles textures
	void BindTexture(unsigned textureNumber)const;
	void UnbindTexture(unsigned textureNumber)const;

	virtual void LoadFromMemory(unsigned x, unsigned y, void* data, RenderDataType type = Render_Data_Type_BGRA_32, RenderDataFormat format = Render_Data_Format_Unsigned_Byte)override;
	virtual void SetRenderDataType(RenderDataType newType)override;
	virtual void SetRenderDataFormat(RenderDataFormat newFormat)override;

	virtual void UseTexture(unsigned long data)const override;
	virtual void StopUsingTexture(unsigned long  data)const override;

	virtual unsigned GetTextureID()override;
};
