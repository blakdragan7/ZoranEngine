#pragma once
#include "WindowBase.h"
class WindowsWindow : public WindowBase
{
private:
	HWND hwnd;
	RECT windowRect;
	HDC  dc;
	LONG windowStyle;
	LONG windowEXStyle;

private:
	void SetSizeAndPositionByRect(RECT rect);
public:
	WindowsWindow(RenderEngine* engine);
	~WindowsWindow();

	virtual bool MakeWindow(const char* title,int x, int y, int w, int h)override;
	virtual void SetWindowFullScreen(bool isFullScreen)override;
	virtual void SetPosition(int x, int y)override;
	virtual void SetSize(int w, int h)override;
	virtual void MakeActive()override;
	virtual void SwapBuffers()override;

};

