#include "stdafx.h"
#include "ZGIPanel.h"

#include <Core/PlatformMouseBase.h>

ZGIPanel::ZGIPanel(): drawDebugView(false), drawEditorView(false)
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
	ZGIWidget* widget = WidgetForPosition(m->GetWindowSpacePosition());
	if(widget == 0)widget = WidgetForPosition(m->GetWindowSpacePosition() - m->GetWindowSpaceDelta());

	if (widget)
	{
		widget->MouseMove(m);
		return true;
	}

	return false;
}

bool ZGIPanel::MouseDown(const PlatformMouseBase *m)
{
	if (ZGIWidget* w = WidgetForPosition(m->GetWindowSpacePosition()))
	{
		w->MouseDown(m);
		return true;
	}

	return false;
}

bool ZGIPanel::MouseUp(const PlatformMouseBase *m)
{
	if (ZGIWidget* w = WidgetForPosition(m->GetWindowSpacePosition()))
	{
		w->MouseUp(m);
		return true;
	}

	return false;
}