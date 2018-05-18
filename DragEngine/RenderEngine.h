#pragma once
#include "PlatformTypes.h"
#include <map>
/*
RenderEngine, this is an abstract base class that allows common functionality across different render types such as OpenGL vs DirectX
*/
class SceneObject;
class DRAGENGINE_EXPORT RenderEngine
{
public:
	RenderEngine();
	virtual ~RenderEngine();

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
	virtual void RemoveSceneObject(SceneObject* object) = 0;

	virtual void* CreateTexture(const char* path);
};

