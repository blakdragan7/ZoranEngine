#include "stdafx.h"
#include "ZGIPanel.h"

#include <Core/PlatformMouseBase.h>
#include <ZGI/Windows/ZGIVirtualWindow.h>

ZGIPanel::ZGIPanel(ZGIVirtualWindow* owningWindow): ZGIWidget(owningWindow)
{
}

ZGIPanel::~ZGIPanel()
{
}

ZGIWidget * ZGIPanel::HitTest(Vec2D pos)
{
	if (ZGIWidget * w = WidgetForPosition(pos))
		return w;

	return ZGIWidget::HitTest(pos);
}

bool ZGIPanel::MouseMove(const PlatformMouseBase *m)
{
	Vector2D pos = owningWindow->ConvertAbsoluteToVirtual(m->GetPosition());

	ZGIWidget* widget = WidgetForPosition(pos);
	if(widget == 0)widget = WidgetForPosition(pos - m->GetDelta());

	if (widget)
	{
		widget->MouseMove(m);
		return true;
	}

	return ZGIWidget::MouseMove(m);
}

bool ZGIPanel::MouseDown(const PlatformMouseBase *m)
{
	Vector2D pos = owningWindow->ConvertAbsoluteToVirtual(m->GetPosition());

	if (ZGIWidget* w = WidgetForPosition(pos))
	{
		w->MouseDown(m);
		return true;
	}

	return false;
}

bool ZGIPanel::MouseUp(const PlatformMouseBase *m)
{
	Vector2D pos = owningWindow->ConvertAbsoluteToVirtual(m->GetPosition());

	if (ZGIWidget* w = WidgetForPosition(pos))
	{
		w->MouseUp(m);
		return true;
	}

	return false;
}

bool ZGIPanel::KeyEvent(KeyEventType type, unsigned key)
{
	return KeyEventSub(type, key);
}
