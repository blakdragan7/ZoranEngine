#pragma once
#include "PlatformTypes.h"
#include "RenderEngine.h"
class DRAGENGINE_EXPORT OpenGLRenderEngine : public RenderEngine
{

public:
	OpenGLRenderEngine();
	~OpenGLRenderEngine();

	virtual void InitEngine(WindowHandle handle)override;

	virtual void DisableAlpha()override;
	virtual void EnableAlpha()override;
	virtual void ClearBuffers()override;
	virtual void DrawAll()override;
	virtual void Resize(int x, int y)override;
};

