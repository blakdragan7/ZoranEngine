#pragma once
#include "ZGIWidget.h"
#include <Rendering/Primitives.h>

class ZGICursor : public ZGIWidget
{
private:
	Vector2D alphaRange;
	Color currentColor;

	float currentTime;
	float solidTime;

public:
	float animationTime;

public:
	ZGICursor(ZGIVirtualWindow* owningWindow);
	ZGICursor(Vector2D alphaRange, ZGIVirtualWindow* owningWindow);
	~ZGICursor();

	virtual void Animate(float dt)override;

	void SetTempSolid(float time = 0.3f);

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;
	virtual const char* GetClassString()const override { return "ZGICursor"; }
};

