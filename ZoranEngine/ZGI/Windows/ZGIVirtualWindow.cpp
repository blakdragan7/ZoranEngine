#include "stdafx.h"
#include "ZGIVirtualWindow.h"

#include <ZGI/Core/ZGIVirtualViewport.h>
#include <ZGI/Widgets/ZGIWidget.h>

#include <Core/PlatformMouseBase.h>

#include <Utils/VectorAddons.hpp>
#include <ZGI/Core/ZGIBrush.h>

enum WindowEdge
{
	Window_Edge_None	= 0,
	Window_Edge_Right	= 1 << 0,
	Window_Edge_Top		= 1 << 1,
	Window_Edge_Bottom	= 1 << 2,
	Window_Edge_Left	= 1 << 3
};

unsigned  ZGIVirtualWindow::EdgeForPosition(Vec2D pos, float edgeThresh)
{
	Vec2D windowPos = this->GetWindowPosition();
	Vec2D size = this->GetWindowSize();
	
	unsigned edges = Window_Edge_None;

	if (abs(pos.x - windowPos.x) < edgeThresh)
	{
		edges |= Window_Edge_Left;
	}
	if (abs(pos.x - (windowPos.x + size.w)) < edgeThresh)
	{
		edges |= Window_Edge_Right;
	}
	if (abs(pos.y - windowPos.y) < edgeThresh)
	{
		edges |= Window_Edge_Bottom;
	}
	if (abs(pos.y - (windowPos.y + size.h)) < edgeThresh)
	{
		edges |= Window_Edge_Top;
	}

	return edges;
}

ZGIVirtualWindow::ZGIVirtualWindow(Vec2D pos, Vec2D size, Vec2I OSWindowSize, ZGIVirtualWindow* parent) :
	isBeingResized(false), isResizable(false), parent(parent), rootContent(0), edgeSize(20), shouldRenderBrush(false), currentlySelectedEdge(Window_Edge_None)
{
	subWindows = new std::vector<ZGIVirtualWindow*>();
	viewport = new ZGIVirtualViewport(pos, size, OSWindowSize);
	brush = new ZGIBrush;
}

ZGIVirtualWindow::~ZGIVirtualWindow()
{
	delete viewport;
	
	for (auto& subWindow : *subWindows)
	{
		delete subWindow;
	}

	delete brush;

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

void ZGIVirtualWindow::SetBackgroundColor(const Color & color)
{
	brush->SetBackgroudHue(color);
	shouldRenderBrush = true;
}

void ZGIVirtualWindow::SetBackgroundImage(TextureBase * texture)
{
	brush->SetBackgroudImage(texture);
	shouldRenderBrush = true;
}

Vector2D ZGIVirtualWindow::ConvertAbsoluteToVirtual(Vec2D pos) const
{
	Vec2D position = viewport->GetPosition();
	return pos - globalOffsetCache - position;
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

	if(shouldRenderBrush)brush->RenderBrush(Matrix44::IdentityMatrix);

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
	if (isResizable)
	{
		Vector2D pos = m->GetPosition();

		if (parent)pos = parent->ConvertAbsoluteToVirtual(pos);

		currentlySelectedEdge = EdgeForPosition(pos, edgeSize);
		if (currentlySelectedEdge != Window_Edge_None)return true;
	}

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
	if (isResizable && currentlySelectedEdge != Window_Edge_None)
	{
		Vec2D position = viewport->GetPosition();
		Vector2D size = Vector2D(viewport->GetSize()); // force copy

		if (currentlySelectedEdge == Window_Edge_Top)
		{
			this->ResizeVirtualWindow({ size.x, size.y + m->GetDelta().y });
		}
		else if(currentlySelectedEdge == Window_Edge_Right)
		{
			this->ResizeVirtualWindow({ size.x + m->GetDelta().x, size.y });
		}
		else if(currentlySelectedEdge == Window_Edge_Left)
		{
			this->ResizeVirtualWindow({ size.x - m->GetDelta().x, size.y });
			this->SetWindowPosition({ position.x + m->GetDelta().x, position.y });
		}
		else if (currentlySelectedEdge == Window_Edge_Bottom)
		{
			this->ResizeVirtualWindow({ size.x, size.y - m->GetDelta().y });
			this->SetWindowPosition({ position.x, position.y + m->GetDelta().y });
		}
		else if (currentlySelectedEdge & Window_Edge_Top && currentlySelectedEdge & Window_Edge_Right)
		{
			this->ResizeVirtualWindow({ size.x + m->GetDelta().x, size.y + m->GetDelta().y });
		}
		else if (currentlySelectedEdge & Window_Edge_Top && currentlySelectedEdge & Window_Edge_Left)
		{
			this->ResizeVirtualWindow({ size.x - m->GetDelta().x, size.y + m->GetDelta().y });
			this->SetWindowPosition({ position.x + m->GetDelta().x, position.y });
		}
		else if (currentlySelectedEdge & Window_Edge_Bottom && currentlySelectedEdge & Window_Edge_Right)
		{
			this->ResizeVirtualWindow({ size.x + m->GetDelta().x, size.y - m->GetDelta().y });
			this->SetWindowPosition({ position.x, position.y + m->GetDelta().y });
		}
		else if (currentlySelectedEdge & Window_Edge_Bottom&& currentlySelectedEdge & Window_Edge_Left)
		{
			this->ResizeVirtualWindow({ size.x - m->GetDelta().x, size.y - m->GetDelta().y });
			this->SetWindowPosition({ position.x + m->GetDelta().x, position.y + m->GetDelta().y });
		}

		if (rootContent)rootContent->ContainerResized(viewport->GetSize(), size);

		return true;
	}

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
	currentlySelectedEdge = Window_Edge_None;

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

bool ZGIVirtualWindow::RawKeyEvent(KeyEventType type, unsigned key)
{
	if (firstResponder)
		return firstResponder->RawKeyEvent(type, key);

	return false;
}

bool ZGIVirtualWindow::CharEvent(unsigned uni)
{
	if (firstResponder)
		return firstResponder->CharEvent(uni);
	
	return false;
}
