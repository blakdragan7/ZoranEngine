#pragma once
#include "PlatformTypes.h"
#include "RenderEngineBase.h"

#include <map>
#include <vector>

typedef std::pair < ShaderProgramBase*, std::vector<SceneObject*>> GLRenderMapPair;

class DRAGENGINE_EXPORT OpenGLRenderEngine : public RenderEngineBase
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

	virtual TextureBase* CreateTexture(const char* path, RenderDataType bufferType, RenderDataFormat bufferFormat)override;
	virtual TextureBase* CreateTexture(void* data, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2L size)override;
	virtual RenderedObjectBase* CreateRenderedObject()override;
	virtual bool CreateFrameBuffer(FrameBufferBase** outBuffer, TextureBase** outTexture, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2L size)override;
	virtual ShaderProgramBase* CreateShaderProgram(const char* vertex, const char* fragment);

	virtual void CheckErrors(const char* text)override;
	virtual void ClearErrors()override;
};
