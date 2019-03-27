#include "stdafx.h"
#include "ZGIButton.h"

#include <Interfaces/IZGIButtonEventHandler.h>
#include <ZGI/Core/ZGIBrush.h>

#include <Rendering/Renderers/GUIRenderer.h>
#include <Rendering/RenderEngineBase.h>

ZGIButton::ZGIButton(ZGIVirtualWindow* owningWindow) : ButtonStopedHoveredFunction(0), ButtonHoveredFunction(0), ButtonPressedFunction(0), ButtonReleasedFunction(0),
	currentState(Button_State_None), eventHandler(0), ZGIWidget(owningWindow)
{
	standardColor = { 0.6f,0.6f,0.6f,0.6f };
	hoveredColor = { 0.8f,0.8f,0.8f,0.8f };
	pressedColor = { 0.4f,0.4f,0.4f,0.4f };

	widgetBrush->SetBackgroudHue(standardColor);
	shouldDrawBrush = true;

	setColorOnHover = true;
}

ZGIButton::~ZGIButton()
{
}

void ZGIButton::SetColorOnMouseEvent(bool shouldSetColor)
{
	setColorOnHover = shouldSetColor;
}

void ZGIButton::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: Ancher Stuff
}

bool ZGIButton::MouseDown(const PlatformMouseBase & mouse)
{
	if (setColorOnHover)widgetBrush->SetBackgroudHue(pressedColor);

	if (currentState != Button_State_Pressed)
	{
		currentState = Button_State_Pressed;
		if (eventHandler)eventHandler->ButtonPressed(this);
		else if (ButtonPressedFunction)ButtonPressedFunction(this);
		else ButtonPressed();
	}
	return false;
}

bool ZGIButton::MouseUp(const PlatformMouseBase & mouse)
{
	if(setColorOnHover)widgetBrush->SetBackgroudHue(standardColor);
	
	currentState = Button_State_None;
	if (eventHandler)eventHandler->ButtonReleased(this);
	else if (ButtonReleasedFunction)ButtonReleasedFunction(this);
	else ButtonReleased();
	
	return false;
}

bool ZGIButton::MouseEnterd(const PlatformMouseBase & mouse)
{
	if (setColorOnHover)widgetBrush->SetBackgroudHue(hoveredColor);
	if (currentState != Button_State_Hovered)
	{
		currentState = Button_State_Hovered;
		if (eventHandler)eventHandler->ButtonHovered(this);
		else if (ButtonHoveredFunction)ButtonHoveredFunction(this);
		else ButtonWasHovered();
	}
	return false;
}

bool ZGIButton::MouseLeft(const PlatformMouseBase & mouse)
{
	if (setColorOnHover)widgetBrush->SetBackgroudHue(standardColor);
	
	currentState = Button_State_None;
	if (eventHandler)eventHandler->ButtonStopedBeingHovered(this);
	else if (ButtonStopedHoveredFunction)ButtonStopedHoveredFunction(this);
	else ButtonStopedHovered();
	
	return false;
}
