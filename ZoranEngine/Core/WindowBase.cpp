#include "stdafx.h"

#include "WindowBase.h"
#include <ZGI/Core/ZGIVirtualWindow.h>
#include <Rendering/RenderEngineBase.h>

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

void WindowBase::SetPosition(Vec2I position)
{
	this->SetPosition(position.x,position.y);
}

void WindowBase::SetSize(Vec2I size)
{
	this->SetSize(size.w, size.h);
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
