#pragma once
class ZGIButton;
class IZGIButtonEventHandler
{
public:
	virtual void ButtonPressed(ZGIButton*) = 0;
	virtual void ButtonReleased(ZGIButton*) = 0;
	virtual void ButtonHovered(ZGIButton*) = 0;
	virtual void ButtonStopedBeingHovered(ZGIButton*) = 0;
};

