#pragma once
#include "ZGILabel.h"

class ZGICursor;
class ZoranEngine_EXPORT ZGITextEdit : public ZGILabel
{
private:
	ZGICursor* cursor;
	int cursorPos;
	bool needsCursorUpdate;

private:
	void UpdateCursorFromPos();

public:
	ZGITextEdit(ZGIVirtualWindow* owningWinodow);
	~ZGITextEdit();

	virtual void Render(const Matrix44& projection)override;
	virtual void Animate(float dt)override;

	virtual bool RawKeyEvent(KeyEventType type, unsigned uni)override;
	virtual bool CharEvent(unsigned uni)override;

	virtual bool MouseUp(const PlatformMouseBase*)override;
	virtual const char* GetClassString()const override { return "ZGITextEdit"; }
};

