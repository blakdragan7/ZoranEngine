#pragma once
#include <Rendering/RenderEngine2DBase.h>

#include <unordered_map>
#include <map>
#include <vector>

typedef std::unordered_map  <ShaderProgramBase*, std::vector<Visible2DComponent*>> GL2DRenderMap;
typedef std::pair <ShaderProgramBase*, std::vector<Visible2DComponent*>> GL2DRenderMapPair;
typedef std::map<unsigned,GL2DRenderMap*> GL2DRenderLayers;
 
class OpenGLContext;
class OpenGL2DRenderEngine : public RenderEngine2DBase
{
private:
	/* Used to render opengl object from back to front, z sorted */
	GL2DRenderLayers * renderLayers;
	OpenGLContext* context;

public:
	OpenGL2DRenderEngine();
	~OpenGL2DRenderEngine();

	virtual void SetViewport(int x, int y, int width, int height)override;

	virtual void ClearErrors()override;
	virtual void EnableAlpha()override;
	virtual void DisableAlpha()override;
	virtual void ClearBuffers()override;
	virtual void Resize(int w, int h)override;
	virtual void EnableDepthTesting()override;
	virtual void DisableDepthTesting()override;
	virtual void SetLineWidth(float width)override;
	virtual void CheckErrors(const char* text)override;
	virtual void InitEngine(WindowHandle handle)override;
	virtual TextureBase* CreateTexture(const char* path, RenderDataType bufferType, RenderDataFormat bufferFormat, int* error)override;
	virtual TextureBase* CreateTexture(void* data, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2I size)override;
	virtual bool CreateFrameBuffer(FrameBufferBase** outBuffer, ImageResource* outTexture, Vec2I size, RenderDataType bufferType = Render_Data_Type_BGRA_32, RenderDataFormat bufferFormat = Render_Data_Format_Unsigned_Byte)override;

	virtual LineRenderer* CreateLineRenderer()override;
	virtual LineLoopRenderer* CreateLineLoopRenderer()override;
	virtual ModelRenderer* CreateModelRenderer()override;
	virtual QuadRenderer* CreateQuadRenderer()override;
	virtual QuadStripRenderer* CreateQuadStripRenderer()override;
	virtual SpriteRenderer* CreateSpriteRenderer()override;
	virtual TriangleRenderer* CreateTriangleRenderer()override;
	virtual TriangleStripRenderer* CreateTriangleStripRenderer()override;
	virtual FontRenderer* CreateFontRenderer(FontResource font)override;
	virtual GUIRenderer* CreateGUIRenderer()override;
	virtual GUIColoredRectRenderer* CreateGUIColoredRectRenderer()override;

	virtual void DrawScene(const Matrix44& cameraMatrix)override;

	virtual void AddComponent(Visible2DComponent* component)override;
	virtual bool RemoveComponent(Visible2DComponent* component)override;

	virtual const char* GetEngineReadableName()const override { return "OpenGL 2D RenderEngine"; };

	virtual ShaderProgramBase* StandardTexturedShader()override;
};
