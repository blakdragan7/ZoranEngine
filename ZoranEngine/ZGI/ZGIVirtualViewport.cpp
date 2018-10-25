#include "stdafx.h"
#include "ZGIVirtualViewport.h"

#include <Rendering/RenderEngineBase.h>

ZGIVirtualViewport::ZGIVirtualViewport(Vec2D position, Vec2D size, Vec2I windowSize) : position(position), size(size), cachedWindowSize(windowSize)
{
	normalizedPosition = { (float)position.x / (float)windowSize.w, (float)position.y / (float)windowSize.h };
	normalizedSize = { (float)size.x / (float)windowSize.w, (float)size.y / (float)windowSize.h };

	projectionMatrix = Matrix44::OrthoMatrix(0.0f, (float)size.w, 0.0f, (float)size.h);
}

ZGIVirtualViewport::~ZGIVirtualViewport()
{
}

void ZGIVirtualViewport::WindowResized(Vec2I newWindowSize)
{
	position = newWindowSize * normalizedPosition;
	size = newWindowSize * normalizedSize;
	cachedWindowSize = newWindowSize;

	projectionMatrix = Matrix44::OrthoMatrix(0.0f, (float)size.w, 0.0f, (float)size.h);
}

void ZGIVirtualViewport::SetViewportActive(Vec2D globalOffset)
{
	
	rEngine->SetViewport((int)(globalOffset.x + position.x), (int)(globalOffset.y + position.y), (int)size.w, (int)size.h);
	
}

void ZGIVirtualViewport::SetSize(Vec2D newSize)
{
	this->size = newSize;
	normalizedSize = newSize / cachedWindowSize;

	projectionMatrix = Matrix44::OrthoMatrix(0.0f, (float)size.w, 0.0f, (float)size.h);
}

void ZGIVirtualViewport::SetPosition(Vec2D newPosition)
{
	this->position = newPosition;
	normalizedPosition = newPosition / cachedWindowSize;
}
