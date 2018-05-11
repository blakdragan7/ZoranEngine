#pragma once
#include "PlatformTypes.h"
/*
	WindowBase is an abstract base class that defines functions that all windows should have
*/

class RenderEngine;
class DRAGENGINE_EXPORT WindowBase
{
protected:
	RenderEngine* renderEngine;
	WindowHandle windowHandle;

public:
	WindowBase(RenderEngine* engine);
	virtual ~WindowBase();

	virtual void MakeWindow(const char* title,int x,int y,int w,int h) = 0;
	virtual void MakeFullScreen() = 0;
	virtual void SetPosition(int x, int y) = 0;
	virtual void SetSize(int w,int h) = 0;
	virtual void MakeActive() = 0;
	virtual void GetPosition() = 0;
	virtual inline WindowHandle GetHandle() { return windowHandle; }
	virtual void SwapBuffers() = 0;
	virtual void MainDraw();
};

