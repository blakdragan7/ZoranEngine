#pragma once
#include <Rendering/RenderEngine3DBase.h>

typedef std::unordered_map  <ShaderProgramBase*, std::vector<Visible3DComponent*>> GL3DRenderMap;
typedef std::pair <ShaderProgramBase*, std::vector<Visible3DComponent*>> GL3DRenderMapPair;
class OpenGLContext;
class OpenGL3DRenderEngine : public RenderEngine3DBase
{
private:
	GL3DRenderMap * renderMap;
	OpenGLContext* context;

public:
	OpenGL3DRenderEngine();
	~OpenGL3DRenderEngine();

	virtual void ClearErrors()override;
	virtual void EnableAlpha()override;
	virtual void DisableAlpha()override;
	virtual void ClearBuffers()override;
	virtual void Resize(int w, int h)override;
	virtual void EnableDepthTesting()override;
	virtual void DisableDepthTesting()override;
	virtual void SetLineWidth(float width)override;
	virtual void CheckErrors(const char* text)override;
	virtual TextureBase* CreateTexture(const char* path, RenderDataType bufferType, RenderDataFormat bufferFormat)override;
	virtual TextureBase* CreateTexture(void* data, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2I size)override;
	virtual bool CreateFrameBuffer(FrameBufferBase** outBuffer, TextureBase** outTexture,
		RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2I size)override;

	virtual void DrawAll()override;

	virtual void InitEngine(WindowHandle handle)override;

	virtual void AddComponent(Visible3DComponent* component)override;
	virtual bool RemoveComponent(Visible3DComponent* component)override;
	virtual const char* GetEngineReadableName() { return "OpenGL 3D RenderEngine"; };

	virtual LineRenderer* CreateLineRenderer()override;
	virtual LineLoopRenderer* CreateLineLoopRenderer()override;
	virtual ModelRenderer* CreateModelRenderer()override;
	virtual QuadRenderer* CreateQuadRenderer()override;
	virtual QuadStripRenderer* CreateQuadStripRenderer()override;
	virtual SpriteRenderer* CreateSpriteRenderer()override;
	virtual TriangleRenderer* CreateTriangleRenderer()override;
	virtual TriangleStripRenderer* CreateTriangleStripRenderer()override;
};

