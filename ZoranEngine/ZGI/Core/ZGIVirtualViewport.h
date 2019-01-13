#pragma once

#include <ZGI\Core\ZGIBase.h>

class ZGIVirtualViewport : public ZGIBase
{
private:
	Vector2I cachedWindowSize;
		   
	Vector2D size;
	Vector2D position;

	Matrix44 projectionMatrix;

	Vector2D normalizedSize;
	Vector2D normalizedPosition;

public:
	ZGIVirtualViewport(Vec2D position, Vec2D size, Vec2I windowSize);
	virtual ~ZGIVirtualViewport();

	void WindowResized(Vec2I newWindowSize);
	void SetViewportActive(Vec2D globalOffset);

	void SetSize(Vec2D newSize);
	void SetPosition(Vec2D newPosition);

	inline Vec2D GetPosition()const { return position; }
	inline Vec2D GetSize()const { return size; }

	inline const Matrix44& GetProjectionMatrix()const { return projectionMatrix; }

};

