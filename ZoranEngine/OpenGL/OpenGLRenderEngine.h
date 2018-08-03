#pragma once
#include "Core/PlatformTypes.h"
#include "Rendering/RenderEngineBase.h"

#include <map>
#include <vector>

typedef std::pair < ShaderProgramBase*, std::vector<SceneObject*>> GLRenderMapPair;

class ZoranEngine_EXPORT OpenGLRenderEngine : public RenderEngineBase
{
private:
	// in opengl we order the renderd object by shader program because it is much more effecient for rendering
#pragma warning(push)
#pragma warning(disable:4251)
	std::map<ShaderProgramBase*, std::vector<SceneObject*>> renderMap;
#pragma warning(pop)

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

	virtual void AddSceneObject(SceneObject* object);
	virtual bool RemoveSceneObject(SceneObject* object);

	virtual void SetupScene(Vector3D center, Vector3D size)override;

	virtual TextureBase* CreateTexture(const char* path, RenderDataType bufferType, RenderDataFormat bufferFormat)override;
	virtual TextureBase* CreateTexture(void* data, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2I size)override;
	virtual RenderedObjectBase* CreateRenderedObject()override;
	virtual bool CreateFrameBuffer(FrameBufferBase** outBuffer, TextureBase** outTexture, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2I size)override;
	virtual ShaderProgramBase* CreateShaderProgram(const char* vertex, const char* fragment);

	virtual void SetLineWidth(float width);

	virtual void CheckErrors(const char* text)override;
	virtual void ClearErrors()override;
};
