#pragma once
#include <ZGI/Widgets/ZGIWidget.h>
#include <Interfaces/IZGIButtonEventHandler.h>

class ZGILabel;
class ZGIButton;
class ZGIGridPanel;
class HighPrecisionClock;
class IZGISpinBoxEventHandler;
class ZoranEngine_EXPORT ZGISpinBox : public ZGIWidget, public IZGIButtonEventHandler
{
private:
	float val;
	float shiftVal;

	IZGISpinBoxEventHandler* handler;

	HighPrecisionClock*	clock;

	ZGIButton* upButton;
	ZGIButton* downButton;
	ZGILabel* valueLabel;

	ZGIGridPanel* panel;

	bool isWholeNumber;
	int spinDirection;

protected:
	virtual void ValueChanged(ZGISpinBox* b, float val);

public:
	ZGISpinBox(bool isWholeNumber, float val, float shiftVal, ZGIVirtualWindow* owningWindow);
	~ZGISpinBox();

	void SetIsWholeNumber(bool isWholeNumber_);

	// directly sets value of val
	void SetVal(float newVal);
	// increments value of val by byval i.e. val += byVal
	void IncVal(float byVal);

	inline void SetShiftVal(float shift) { shiftVal = shift; }

	inline void SetEventHandler(IZGISpinBoxEventHandler* handler_)
	{handler = handler_;}

	// Widget Overrides
	virtual void Render(const Matrix44& projection)override;

	virtual void ContainerResized(Vec2D newSize, Vec2D oldSize)override;
	virtual const char* GetClassString()const override { return "ZGISpinBox"; }

	virtual ZGIWidget* HitTest(Vec2D pos)override;

	// button handler

	virtual void ButtonPressed(ZGIButton*)override;
	virtual void ButtonReleased(ZGIButton*)override;
	virtual void ButtonHovered(ZGIButton*)override;
	virtual void ButtonStopedBeingHovered(ZGIButton*)override;
};
