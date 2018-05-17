#pragma once
#include "PlatformTypes.h"
#include "Vector2.h"
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
	Vec2L position;
	Vec2L size;

protected:
	void inline SetWindowPosNoExecute(int x, int y) { position.x = x; position.y = y; }
	void inline SetWindowSizeNoExecute(int w, int h) { size.w = w; size.h = h; }
	void Resize(Vec2L size);

public:
	WindowBase(RenderEngine* engine);
	virtual ~WindowBase();

	void SetPosition(Vec2L position);
	void SetSize(Vec2L size);
	void MakeWindow(const char* title, Vec2L position, Vec2L size);
	virtual bool MakeWindow(const char* title,int x,int y,int w,int h) = 0;
	virtual void SetWindowFullScreen(bool isFullScreen) = 0;
	virtual void SetPosition(long x, long y) = 0;
	virtual void SetSize(long w,long h) = 0;
	virtual void MakeActive() = 0;
	inline Vec2L GetPosition() { return position; };
	inline Vec2L GetSize() { return size; };
	virtual inline WindowHandle GetHandle() { return windowHandle; }
	virtual void SwapBuffers() = 0;
	virtual void MainDraw();

	inline bool IsFullScreen() { return isFullScreen; };
	inline bool IsMaxamized() { return isMaximized; };
};

