#include "stdafx.h"

#include "WindowBase.h"
#include <ZGI/Windows/ZGIVirtualWindow.h>
#include <Rendering/RenderEngineBase.h>
#include <Core/PlatformMouseBase.h>

WindowBase::WindowBase(ZoranEngine* zoranEngine)
{
	isFullScreen = false;
	isMaximized = false;
	windowHandle = 0;
	rootVirtualWindow = 0;
	m = 0;
}

WindowBase::~WindowBase()
{
	if (rootVirtualWindow)delete rootVirtualWindow;
	if(m)delete m;
}

void WindowBase::Resize(Vec2I size)
{
	SetWindowSizeNoExecute(size.w,size.h);
	if(rEngine)rEngine->Resize(size.w, size.h);
	zEngine->ScreenResized(static_cast<float>(size.w), static_cast<float>(size.h));
	if (rootVirtualWindow)rootVirtualWindow->OSWindowWasResized(size);
}

bool WindowBase::UniIsChar(unsigned uni)
{
	switch (uni)
	{
	case Key_F1:
	case Key_F2:
	case Key_F3:
	case Key_F4:
	case Key_F5:
	case Key_F6:
	case Key_F7:
	case Key_F8:
	case Key_F9:
	case Key_F10:
	case Key_F11:
	case Key_F12:
	case Key_Up_Arrow:
	case Key_Down_Arrow:
	case Key_Right_Arrow:
	case Key_Left_Arrow:
	case Key_BackSpace:
	case Key_Delete:
	case Key_Enter:
	case Key_PrintScreen:
	case Key_Esc:
	case Key_Shift:
	case Key_Tab:
	case Key_CapsLock:
	case Key_Insert:
		return false;
		break;

	default:
		return true;
	}
}

void WindowBase::SetPosition(Vec2I position)
{
	this->SetPosition(position.x,position.y);
}

void WindowBase::SetSize(Vec2I size)
{
	this->SetSize(size.w, size.h);
	if (rootVirtualWindow)rootVirtualWindow->OSWindowWasResized(size);
}

void WindowBase::MakeWindow(const char * title, Vec2I position, Vec2I size)
{
	this->MakeWindow(title,position.x, position.y,size.w,size.h);
}

void WindowBase::MainDraw()
{
	if (rootVirtualWindow)rootVirtualWindow->RenderWindow(Vector2I::Zero);
	zEngine->DrawStep(); 
	SwapBuffers();
}

void WindowBase::SetRootVirtualWindow(ZGIVirtualWindow * newWindow)
{
	if (rootVirtualWindow)delete rootVirtualWindow;
	rootVirtualWindow = newWindow;
}
