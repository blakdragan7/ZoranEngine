#pragma once
#include "Core/PlatformTypes.h"
#include "Rendering/RenderEngineBase.h"

class ZoranEngine_EXPORT OpenGLRenderEngine : public RenderEngineBase
{
private:
	// in opengl we order the renderd object by shader program because it is much more effecient for rendering

	void* context;

public:
	OpenGLRenderEngine();
	~OpenGLRenderEngine();

	virtual void InitEngine(WindowHandle handle)override;

	virtual void EnableDepthTesting()override;
	virtual void DisableDepthTesting()override;
	virtual void DisableAlpha()override;
	virtual void EnableAlpha()override;
	virtual void ClearBuffers()override;
	virtual void Resize(int x, int y)override;

	virtual RenderedObjectBase* CreateRenderedObject()override;
	virtual TextureBase* CreateTexture(const char* path, RenderDataType bufferType, RenderDataFormat bufferFormat)override;
	virtual TextureBase* CreateTexture(void* data, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2I size)override;
	virtual bool CreateFrameBuffer(FrameBufferBase** outBuffer, TextureBase** outTexture, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2I size)override;

	virtual void SetLineWidth(float width);

	virtual void CheckErrors(const char* text)override;
	virtual void ClearErrors()override;

	virtual const char* GetEngineReadableName() { return "Unimplemented OpenGL RenderEngine"; };
};
