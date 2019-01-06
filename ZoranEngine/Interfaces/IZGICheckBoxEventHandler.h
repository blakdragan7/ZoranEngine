#pragma once
class ZGICheckBox;
class IZGICheckBoxEventHandler
{
public:
	virtual void WasChecked(ZGICheckBox*, bool checked) = 0;
};

