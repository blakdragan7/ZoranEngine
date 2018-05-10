#pragma once
#include "PlatformTypes.h"
/*
	RenderEngine, this is an abstract base class that allows common functionality across different render types such as OpenGL vs DirectX
*/
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
};

