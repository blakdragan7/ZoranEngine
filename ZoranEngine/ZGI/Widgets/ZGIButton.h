#pragma once
#include <ZGI/Widgets/ZGIWidget.h>
#include <functional>
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
	// TODO: test this in multi platform setting to make sure that it works
#pragma warning(push)
#pragma warning(disable:4251)
	std::function<void(ZGIButton*)> ButtonPressedFunction;
	std::function<void(ZGIButton*)> ButtonReleasedFunction;
	std::function<void(ZGIButton*)> ButtonHoveredFunction;
	std::function<void(ZGIButton*)> ButtonStopedHoveredFunction;
#pragma warning(pop)

	virtual void ButtonPressed() {}
	virtual void ButtonReleased() {}
	virtual void ButtonWasHovered() {}
	virtual void ButtonStopedHovered() {}

public:
	ZGIButton(ZGIVirtualWindow* owningWindow);
	~ZGIButton();

	inline void SetEventHandler(IZGIButtonEventHandler* handler) { eventHandler = handler; }
	inline ZGIButtonState GetState() { return currentState; }

	inline void SetButtonPressedFunction(std::function<void(ZGIButton*)> func) { ButtonPressedFunction = func; }
	inline void SetButtonReleasedFunction(std::function<void(ZGIButton*)> func) { ButtonReleasedFunction = func; }
	inline void SetButtonHoveredFunction(std::function<void(ZGIButton*)> func) { ButtonHoveredFunction = func; }
	inline void SetButtonStopedHoveredFunction(std::function<void(ZGIButton*)> func) { ButtonStopedHoveredFunction = func; }

	// widget override

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;

	virtual bool MouseDown(const PlatformMouseBase& mouse)override;
	virtual bool MouseUp(const PlatformMouseBase& mouse)override;
	virtual bool MouseEnterd(const PlatformMouseBase& mouse)override;
	virtual bool MouseLeft(const PlatformMouseBase& mouse)override;

	virtual const char* GetClassString()const override { return "ZGIButton"; }
};
