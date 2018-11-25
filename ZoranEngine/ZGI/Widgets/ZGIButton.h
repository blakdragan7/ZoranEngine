#pragma once
#include <ZGI\Widgets\ZGIWidget.h>

/*
*	This is a generic button class for ZGI
*	It can be given and interface or be subclassed to implement 
*	events
*
*/

enum ZGIButtonState
{
	Button_State_Pressed,
	Button_State_Hovered,
	Button_State_None
};

class GUIRenderer;
class IZGIButtonEventHandler;
class ZoranEngine_EXPORT ZGIButton : public ZGIWidget
{
private:
	ZGIButtonState currentState;

protected:
	IZGIButtonEventHandler * eventHandler;

	virtual void ButtonPressed() {}
	virtual void ButtonReleased() {}
	virtual void ButtonWasHovered() {}
	virtual void ButtonStopedHovered() {}

public:
	ZGIButton(ZGIVirtualWindow* owningWindow);
	~ZGIButton();

	inline void SetEventHandler(IZGIButtonEventHandler* handler) { eventHandler = handler; }
	inline ZGIButtonState GetState() { return currentState; }

	// widget override

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;

	virtual bool MouseDown(const PlatformMouseBase* mouse)override;
	virtual bool MouseUp(const PlatformMouseBase* mouse)override;
	virtual bool MouseEnterd(const PlatformMouseBase* mouse)override;
	virtual bool MouseLeft(const PlatformMouseBase* mouse)override;
};
