#pragma once
#include "Core/WindowBase.h"
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
	unsigned ConvertWPARAMToKey(WPARAM key);

public:

	WindowsWindow(ZoranEngine* engine);
	~WindowsWindow();

	virtual bool MakeWindow(const char* title,int x, int y, int w, int h)override;
	virtual void SetWindowFullScreen(bool isFullScreen)override;
	virtual void SetPosition(long x, long y)override;
	virtual void SetSize(long w, long h)override;
	virtual void MakeActive()override;
	virtual void SwapBuffers()override;
	virtual void MainDraw()override;

	friend static LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

