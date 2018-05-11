#pragma once
#include "WindowBase.h"
class WindowsWindow : public WindowBase
{
public:
	WindowsWindow(RenderEngine* engine);
	~WindowsWindow();

	virtual void MakeWindow(const char* title,int x, int y, int w, int h)override;
	virtual void MakeFullScreen()override;
	virtual void SetPosition(int x, int y)override;
	virtual void SetSize(int w, int h)override;
	virtual void MakeActive()override;
	virtual void GetPosition()override;
	virtual void SwapBuffers()override;

};

