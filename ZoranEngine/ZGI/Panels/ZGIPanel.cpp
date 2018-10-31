#include "stdafx.h"
#include "ZGIPanel.h"

#include <Core/PlatformMouseBase.h>

ZGIPanel::ZGIPanel()
{
}


ZGIPanel::~ZGIPanel()
{
}

ZGIWidget * ZGIPanel::HitTest(Vec2D pos)
{
	return WidgetForPosition(pos);
}

void ZGIPanel::MouseMove(const PlatformMouseBase *m)
{
	ZGIWidget* widget = WidgetForPosition(m->GetWindowSpacePosition());
	if(widget == 0)widget = WidgetForPosition(m->GetWindowSpacePosition() - m->GetWindowSpaceDelta());

	if (widget)widget->MouseMove(m);
}