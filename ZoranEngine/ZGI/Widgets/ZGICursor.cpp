#include "stdafx.h"
#include "ZGICursor.h"

#include <ZGI/Core/ZGIBrush.h>

ZGICursor::ZGICursor(ZGIVirtualWindow* owningWindow) : solidTime(0), currentTime(0), animationTime(1.0f), ZGIWidget(owningWindow)
{
	alphaRange.x = 0;
	alphaRange.y =1.0f;

	currentColor = { 1.0,1.0,1.0,alphaRange.y };
	GetBrush()->SetBackgroudHue(currentColor);

	SetDrawBrush(true);
}

ZGICursor::ZGICursor(Vector2D alphaRange, ZGIVirtualWindow * owningWindow) : solidTime(0), currentTime(0), animationTime(1.0f), alphaRange(alphaRange), ZGIWidget(owningWindow)
{
	currentColor = {1.0,1.0,1.0,alphaRange.y};
}

ZGICursor::~ZGICursor()
{
}

void ZGICursor::Animate(float dt)
{
	if (solidTime > 0)
	{
		solidTime -= dt;
	}
	else
	{
		currentTime += dt;

		float dta = currentTime / animationTime;

		float da = (dta * dta);

		if (dta >= 1.0f)currentTime = 0;

		currentColor.a = dta < 0.3 ? alphaRange.x : alphaRange.y;

		GetBrush()->SetBackgroudHue(currentColor);
	}
}

void ZGICursor::SetTempSolid(float time)
{
	solidTime = time;
	currentColor.a = alphaRange.y;
	GetBrush()->SetBackgroudHue(currentColor);
}

void ZGICursor::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: ancher stuff
}
