#pragma once
#include "PlatformTypes.h"
/*
	WindowBase is an abstract base class that defines functions that all windows should have
*/

class RenderEngine;
class DRAGENGINE_EXPORT WindowBase
{
private:
	RenderEngine* engine;

public:
	WindowBase(RenderEngine* engine);
	virtual ~WindowBase();

	virtual void MakeFullScreen() = 0;
	virtual void SetPosition() = 0;
	virtual void MakeActive() = 0;
	virtual void GetPosition() = 0;
	virtual WindowHandle GetHandle() = 0;
	virtual void SwapBuffers() = 0;
	virtual void MainDraw();
};

