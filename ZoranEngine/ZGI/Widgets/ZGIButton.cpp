#include "stdafx.h"
#include "ZGIButton.h"

#include <Interfaces/IZGIButtonEventHandler.h>
#include <ZGI/Core/ZGIBrush.h>

#include <Rendering/Renderers/GUIRenderer.h>
#include <Rendering/RenderEngineBase.h>

ZGIButton::ZGIButton(ZGIVirtualWindow* owningWindow) : ButtonStopedHoveredFunction(0), ButtonHoveredFunction(0), ButtonPressedFunction(0), ButtonReleasedFunction(0),
	currentState(Button_State_None), eventHandler(0), ZGIWidget(owningWindow)
{
	widgetBrush->SetBackgroudHue({ 0.7f,0.7f,0.7f,0.0f });
	shouldDrawBrush = true;
}

ZGIButton::~ZGIButton()
{
}

void ZGIButton::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: Ancher Stuff
}

bool ZGIButton::MouseDown(const PlatformMouseBase & mouse)
{
	widgetBrush->SetBackgroudHue({ 0.3f,0.3f,0.3f,0.0f });

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
	widgetBrush->SetBackgroudHue({ 0.7f,0.7f,0.7f,0.0f });
	
	currentState = Button_State_None;
	if (eventHandler)eventHandler->ButtonReleased(this);
	else if (ButtonReleasedFunction)ButtonReleasedFunction(this);
	else ButtonReleased();
	
	return false;
}

bool ZGIButton::MouseEnterd(const PlatformMouseBase & mouse)
{
	widgetBrush->SetBackgroudHue({ 0.9f,0.9f,0.9f,0.0f });
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
	widgetBrush->SetBackgroudHue({ 0.7f,0.7f,0.7f,0.0f });
	
	currentState = Button_State_None;
	if (eventHandler)eventHandler->ButtonStopedBeingHovered(this);
	else if (ButtonStopedHoveredFunction)ButtonStopedHoveredFunction(this);
	else ButtonStopedHovered();
	
	return false;
}
