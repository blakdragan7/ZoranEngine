#pragma once
#include "ZGILabel.h"

class ZoranEngine_EXPORT ZGITextEdit : public ZGILabel
{
public:
	ZGITextEdit(ZGIVirtualWindow* owningWinodow);
	~ZGITextEdit();

	virtual bool RawKeyEvent(KeyEventType type, unsigned uni)override;
	virtual bool CharEvent(unsigned uni)override;

	virtual bool MouseUp(const PlatformMouseBase*)override;
	virtual const char* GetClassString()const override { return "ZGITextEdit"; }
};

