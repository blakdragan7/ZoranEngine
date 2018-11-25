#pragma once
class IZGIButtonEventHandler
{
public:
	virtual void ButtonPressed() = 0;
	virtual void ButtonReleased() = 0;
	virtual void ButtonHovered() = 0;
	virtual void ButtonStopedBeingHovered() = 0;
};

