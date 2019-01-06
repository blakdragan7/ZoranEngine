#pragma once

class IZGISpinBoxEventHandler
{
public:
	virtual void ValueChanged(ZGISpinBox*, float val) = 0;
};
