#pragma once
#include "Core/PlatformTypes.h"
#include "Rendering/RenderEngineBase.h"

#include <map>
#include <vector>

typedef std::map  <ShaderProgramBase*, std::vector<VisibleComponentBase*>> GLRenderMap;
typedef std::pair <ShaderProgramBase*, std::vector<VisibleComponentBase*>> GLRenderMapPair;

class ZoranEngine_EXPORT OpenGLRenderEngine : public RenderEngineBase
{
private:
	// in opengl we order the renderd object by shader program because it is much more effecient for rendering

	GLRenderMap* renderMap;
	void* context;

public:
	OpenGLRenderEngine();
	~OpenGLRenderEngine();

	virtual void InitEngine(WindowHandle handle)override;

	virtual void DisableAlpha()override;
	virtual void EnableAlpha()override;
	virtual void ClearBuffers()override;
	virtual void DrawAll()override;
	virtual void Resize(int x, int y)override;

	virtual void AddComponent(VisibleComponentBase* component)override;
	virtual bool RemoveComponent(VisibleComponentBase* component)override;

	virtual RenderedObjectBase* CreateRenderedObject()override;
	virtual TextureBase* CreateTexture(const char* path, RenderDataType bufferType, RenderDataFormat bufferFormat)override;
	virtual TextureBase* CreateTexture(void* data, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2I size)override;
	virtual bool CreateFrameBuffer(FrameBufferBase** outBuffer, TextureBase** outTexture, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2I size)override;

	virtual void SetLineWidth(float width);

	virtual void CheckErrors(const char* text)override;
	virtual void ClearErrors()override;
};
