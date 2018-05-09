#pragma once
#include "PlatformTypes.h"
/*
	WindowBase is an abstract base class that defines functions that all windows should have
*/

#include <memory>
class RenderEngine;
class WindowBase
{
private:
	std::shared_ptr<RenderEngine> engine;

public:
	WindowBase(std::shared_ptr<RenderEngine> const&engine);
	virtual ~WindowBase();

	virtual void MakeFullScreen() = 0;
	virtual void SetPosition() = 0;
	virtual void MakeActive() = 0;
	virtual void GetPosition() = 0;
	virtual WindowHandle GetHandle() = 0;
	virtual void SwapBuffers() = 0;
	virtual void MainDraw();
};

