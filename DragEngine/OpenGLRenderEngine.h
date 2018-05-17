#pragma once
#include "PlatformTypes.h"
#include "RenderEngine.h"

#include <map>
#include <vector>
class DRAGENGINE_EXPORT OpenGLRenderEngine : public RenderEngine
{
	std::map<unsigned, std::vector<SceneObject*>> RenderMap;

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
	virtual void RemoveSceneObject(SceneObject* object);
};

