#pragma once
#include "Core/PlatformTypes.h"
#include "Rendering/RenderTypes.h"
#include "Math/Vector2.h"
#include <map>
/*
RenderEngine, this is an abstract base class that allows common functionality across different render types such as OpenGL vs DirectX
*/
class SceneObject;
class TextureBase;
class RenderedObjectBase;
class FrameBufferBase;
class ShaderProgramBase;

class ZoranEngine_EXPORT RenderEngineBase
{
public:
	RenderEngineBase();
	virtual ~RenderEngineBase();

	virtual void InitEngine(WindowHandle handle) = 0;

	virtual void DisableAlpha() = 0;
	virtual void EnableAlpha() = 0;
	virtual void ClearBuffers() = 0;
	virtual void DrawAll() = 0;
	virtual void Resize(int w, int h) = 0;

	virtual void SetupScene(Vector3D center, Vector3D size) = 0;

	/* Each implementation must implement way of adding Render Objects 
	*  so DirectX may have a different render loop style then OpenGL */
	virtual void AddSceneObject(SceneObject* object) = 0;
	virtual bool RemoveSceneObject(SceneObject* object) = 0;
	// Creates Texture Memory on GPU and returns a texture object.
	virtual TextureBase* CreateTexture(const char* path, RenderDataType bufferType, RenderDataFormat bufferFormat) = 0;
	virtual TextureBase* CreateTexture(void* data, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2L size) = 0;
	virtual RenderedObjectBase* CreateRenderedObject() = 0;
	// Create frame buffer for offscreen rendering and the texture that represents the result
	virtual bool CreateFrameBuffer(FrameBufferBase** outBuffer, TextureBase** outTexture, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2L size) = 0;
	// Creates a ShaderProgram in gpu memory and returns an instance to it
	virtual ShaderProgramBase* CreateShaderProgram(const char* vertex, const char* fragment) = 0;

	virtual void CheckErrors(const char* text) = 0;
	virtual void ClearErrors() = 0;
};