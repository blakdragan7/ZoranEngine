#include "stdafx.h"
#include "ZGIVirtualWindow.h"

#include <ZGI/Core/ZGIVirtualViewport.h>
#include <ZGI/Widgets/ZGIWidget.h>

#include <Core/PlatformMouseBase.h>

#include <Utils/VectorAddons.hpp>

ZGIVirtualWindow::ZGIVirtualWindow(Vec2D pos, Vec2D size, Vec2I OSWindowSize, ZGIVirtualWindow* parent) : parent(parent), rootContent(0)
{
	subWindows = new std::vector<ZGIVirtualWindow*>();
	viewport = new ZGIVirtualViewport(pos, size, OSWindowSize);
}

ZGIVirtualWindow::~ZGIVirtualWindow()
{
	delete viewport;
	
	for (auto& subWindow : *subWindows)
	{
		delete subWindow;
	}

	delete subWindows;
	
	if (rootContent)delete rootContent;
}

void ZGIVirtualWindow::OSWindowWasResized(Vec2I newSize)
{
	if (rootContent)rootContent->ContainerResized(newSize, viewport->GetSize());
	viewport->WindowResized(newSize);
}

void ZGIVirtualWindow::SetRootContent(ZGIWidget * widget)
{
	if (rootContent)delete rootContent;

	rootContent = widget;
	rootContent->SetSize(viewport->GetSize());
	rootContent->SetPosition({ 0,0 });
}

void ZGIVirtualWindow::AddSubWindow(ZGIVirtualWindow * subWindow)
{
	subWindow->parent = this;
	subWindows->push_back(subWindow);
}

void ZGIVirtualWindow::RemoveSubWindow(ZGIVirtualWindow * subWindow)
{
	if (remove(*subWindows, subWindow))
	{
		subWindow->parent = 0;
	}
}

void ZGIVirtualWindow::SetWindowPosition(Vec2D newPosition)
{
	viewport->SetPosition(newPosition);
}

void ZGIVirtualWindow::ResizeVirtualWindow(Vec2D newSize)
{
	viewport->SetSize(newSize);
}

Vector2D ZGIVirtualWindow::ConvertAbsoluteToVirtual(Vec2D pos) const
{
	return Vector2D(pos.x - globalOffsetCache.x, pos.y - globalOffsetCache.y);
}

Vec2D ZGIVirtualWindow::GetWindowSize() const
{
	return viewport->GetSize();
}

Vec2D ZGIVirtualWindow::GetWindowPosition() const
{
	return viewport->GetPosition();
}

void ZGIVirtualWindow::RenderWindow(Vec2D globalOffset)
{
	globalOffsetCache = globalOffset;

	viewport->SetViewportActive(globalOffset);

	if (rootContent)
	{
		Matrix44 projectionMatrix = viewport->GetProjectionMatrix();
		rootContent->Render(projectionMatrix);
	}

	Vector2D newOffset = globalOffset + GetWindowPosition();
	for (auto& subWindow : *subWindows)
	{
		subWindow->RenderWindow(newOffset);
	}
}

bool ZGIVirtualWindow::MouseDown(const PlatformMouseBase *m)
{
	bool capturedEvent = false;

	for (auto& window : *subWindows)
	{
		if (window->MouseDown(m))
		{
			capturedEvent = true;
			break;
		}
	}

	if(rootContent && capturedEvent == false)
		capturedEvent = rootContent->MouseDown(m);

	return capturedEvent;
}

bool ZGIVirtualWindow::MouseMove(const PlatformMouseBase *m)
{
	bool capturedEvent = false;

	for (auto& window : *subWindows)
	{
		if (window->MouseMove(m))
		{
			capturedEvent = true;
			break;
		}
	}

	if (rootContent && capturedEvent == false)
		capturedEvent = rootContent->MouseMove(m);

	return capturedEvent;
}

bool ZGIVirtualWindow::MouseUp(const PlatformMouseBase *m)
{
	bool capturedEvent = false;

	for (auto& window : *subWindows)
	{
		if (window->MouseUp(m))
		{
			capturedEvent = true;
			break;
		}
	}

	if (rootContent && capturedEvent == false)
		capturedEvent = rootContent->MouseUp(m);

	return capturedEvent;
}

bool ZGIVirtualWindow::MouseEnterd(const PlatformMouseBase *m)
{
	bool capturedEvent = false;

	for (auto& window : *subWindows)
	{
		if (window->MouseEnterd(m))
		{
			capturedEvent = true;
			break;
		}
	}

	if (rootContent && capturedEvent == false)
		capturedEvent = rootContent->MouseEnterd(m);

	return capturedEvent;
}

bool ZGIVirtualWindow::MouseLeft(const PlatformMouseBase *m)
{
	bool capturedEvent = false;

	for (auto& window : *subWindows)
	{
		if (window->MouseLeft(m))
		{
			capturedEvent = true;
			break;
		}
	}

	if (rootContent && capturedEvent == false)
		capturedEvent = rootContent->MouseLeft(m);

	return capturedEvent;
}

bool ZGIVirtualWindow::KeyEvent(KeyEventType type, unsigned key)
{
	bool capturedEvent = false;

	for (auto& window : *subWindows)
	{
		if (window->KeyEvent(type, key))
		{
			capturedEvent = true;
			break;
		}
	}

	if (rootContent && capturedEvent == false)
		capturedEvent = rootContent->KeyEvent(type, key);

	return capturedEvent;
}
