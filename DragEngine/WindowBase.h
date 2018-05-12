#pragma once
#include "PlatformTypes.h"
#include "Vector2D.hpp"
/*
	WindowBase is an abstract base class that defines functions that all windows should have
*/

class RenderEngine;
class DRAGENGINE_EXPORT WindowBase
{
protected:
	RenderEngine* renderEngine;
	WindowHandle windowHandle;
	bool isFullScreen;
	bool isMaximized;
	Vec2I position;
	Vec2I size;

public:
	WindowBase(RenderEngine* engine);
	virtual ~WindowBase();

	void MakeWindow(const char* title, Vec2I position,Vec2I size);
	virtual bool MakeWindow(const char* title,int x,int y,int w,int h) = 0;
	virtual void SetWindowFullScreen(bool isFullScreen) = 0;
	virtual void SetPosition(int x, int y) = 0;
	virtual void SetSize(int w,int h) = 0;
	virtual void MakeActive() = 0;
	inline Vec2I GetPosition() { return position; };
	inline Vec2I GetSize() { return size; };
	virtual inline WindowHandle GetHandle() { return windowHandle; }
	virtual void SwapBuffers() = 0;
	virtual void MainDraw();
};

