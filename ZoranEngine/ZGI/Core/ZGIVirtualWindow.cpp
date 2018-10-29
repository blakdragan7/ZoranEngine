#include "stdafx.h"
#include "ZGIVirtualWindow.h"

#include "ZGIVirtualViewport.h"
#include <ZGI\Widgets\ZGIWidget.h>

#include <Utils/VectorAddons.hpp>

ZGIVirtualWindow::ZGIVirtualWindow(Vec2D pos, Vec2D size, Vec2I OSWindowSize, ZGIVirtualWindow* parent) : parent(parent)
{
	subWindows = new std::vector<ZGIVirtualWindow*>();
	viewport = new ZGIVirtualViewport(pos, size, OSWindowSize);
	rootContent = 0;
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
	rootContent->ContainerResized(newSize, viewport->GetSize());
	viewport->WindowResized(newSize);
}

void ZGIVirtualWindow::SetRootContent(ZGIWidget * widget)
{
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
