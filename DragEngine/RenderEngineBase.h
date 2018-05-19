#pragma once
#include "PlatformTypes.h"
#include "RenderTypes.h"
#include "Vector2.h"
#include <map>
/*
RenderEngine, this is an abstract base class that allows common functionality across different render types such as OpenGL vs DirectX
*/
class SceneObject;
class TextureBase;
class VertexBufferBase;
class FrameBufferBase;
class ShaderProgramBase;

class DRAGENGINE_EXPORT RenderEngineBase
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

	/* Each implementation must implement way of adding Render Objects 
	*  so DirectX may have a different render loop style then OpenGL */
	virtual void AddSceneObject(SceneObject* object) = 0;
	// Scene Object MUST NEVER BE DELETED OUTSIDE OF THIS FUNCTION
	// Each implementation must handle detroying objects individually
	// If returns true then the sceneobject passed is deleted, false if it failed to find it
	virtual bool RemoveSceneObject(SceneObject* object) = 0;
	// Creates Texture Memory on GPU and returns a texture object.
	virtual TextureBase* CreateTexture(const char* path, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2L size) = 0;
	virtual TextureBase* CreateTexture(void* data, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2L size) = 0;
	// Createx Vertex Buffer in GPU Memory
	virtual VertexBufferBase* CreateVertexBuffer(RenderDataType bufferType,RenderDataFormat bufferFormat, Vec2L size,void* data=0) = 0;
	// Create frame buffer for offscreen rendering and the texture that represents the result
	virtual bool CreateFrameBuffer(FrameBufferBase** outBuffer, TextureBase** outTexture, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2L size) = 0;
	// Creates a ShaderProgram in gpu memory and returns an instance to it
	virtual ShaderProgramBase* CreateShaderProgram(const char* vertex, const char* fragment) = 0;
};
