#pragma once
#include "Core/PlatformTypes.h"
#include "Math/Vector2.h"
/*
	WindowBase is an abstract base class that defines functions that all windows should have
*/

class ZoranEngine;
class RenderEngineBase;
class ZoranEngine_EXPORT WindowBase
{
protected:
	ZoranEngine* engine;
	RenderEngineBase* renderEngine;
	WindowHandle windowHandle;
	bool isFullScreen;
	bool isMaximized;
	Vec2I position;
	Vec2I size;

protected:
	void inline SetWindowPosNoExecute(int x, int y) { position.x = x; position.y = y; }
	void inline SetWindowSizeNoExecute(int w, int h) { size.w = w; size.h = h; }
	void Resize(Vec2I size);

public:
	WindowBase(ZoranEngine* zoranEngine);
	virtual ~WindowBase();

	void SetPosition(Vec2I position);
	void SetSize(Vec2I size);
	void MakeWindow(const char* title, Vec2I position, Vec2I size);
	virtual bool MakeWindow(const char* title,int x,int y,int w,int h) = 0;
	virtual void SetWindowFullScreen(bool isFullScreen) = 0;
	virtual void SetPosition(long x, long y) = 0;
	virtual void SetSize(long w,long h) = 0;
	virtual void MakeActive() = 0;
	inline Vec2I GetPosition() { return position; };
	inline Vec2I GetSize() { return size; };
	virtual inline WindowHandle GetHandle() { return windowHandle; }
	virtual void SwapBuffers() = 0;
	virtual void MainDraw();

	inline bool IsFullScreen() { return isFullScreen; };
	inline bool IsMaxamized() { return isMaximized; };
};

